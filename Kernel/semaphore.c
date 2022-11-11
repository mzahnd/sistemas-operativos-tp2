// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**
 * This file is part of sistemas-operativos-tp2
 * Licensed under BSD 3-Clause "New" or "Revised" License.
 * Copyright (c) 2022 Flores Levalle, M.
 *                    López, P.
 *                    Sierra Pérez, C.
 *                    Zahnd, M. E.
 *
 *
 * This implementation is based on Michael Cotterell (@mepcotterell) Gist's
 * on semaphores.
 * See: https://gist.github.com/mepcotterell/6f0a779befe388ab822764255e3776ae
 *
 * In case you do not have the manual entry for stdatomic.h it's available 
 * online: https://en.cppreference.com/w/c/atomic
 */
#include "lib.h"
#include "mem/memory.h"
#ifndef TESTING
#include "scheduler/scheduler.h" /* getCurrentProcessPID(),
                                    unlockProcessByPID(),
                                    lockCurrentProcess() */
#endif

#include "semaphore.h"

/* ------------------------------ */

#define acquire(lock) while (atomic_flag_test_and_set(lock))
#define try_acquire(lock) atomic_flag_test_and_set(lock)
#define release(lock) atomic_flag_clear(lock)

/* ------------------------------ */

typedef struct {
        uint64_t k; // hash
        sosem_t *v; // semaphore
} sosem_kv_t;

typedef struct SOSEM_NODE_T {
        sosem_t *sem; // Current semaphore
        struct SOSEM_NODE_T *next;
        struct SOSEM_NODE_T *prev;
} sosem_node_t;

static sosem_kv_t sosem_names_table[SEM_MAX_NAMED] = {};

// For Userland.
// See function sosem_getinformation()
static sosem_node_t *list_sosem_head = NULL;
static sosem_node_t *list_sosem_tail = NULL;

/* ------------------------------ */

static int get_semaphore_index_from_table(const char *name);
static int add_semaphore_to_table(sosem_t **sem);
static int remove_semaphore_from_table(unsigned int index);
static int create_named_semaphore(const char *name, unsigned int initial_value,
                                  sosem_t **sem);
static inline void userland_init(sosem_t *restrict sem,
                                 sosem_info_t *restrict info);
static inline void userland_destroy(sosem_t *restrict sem);
static inline void userland_update(sosem_t *restrict sem);

static inline void pid_queue_init(sosem_t *restrict sem);
static inline int pid_queue_shift(sosem_t *restrict sem, uint64_t *pid);
static inline int pid_queue_push(sosem_t *restrict sem, uint64_t pid);

static inline int list_sosem_push(sosem_t *restrict sem);
static inline int list_sosem_remove(sosem_t *restrict sem);

#ifdef TESTING
// Replace Scheduler/scheduler.h functions with dummies
static uint64_t getCurrentProcessPID();
static void unlockProcessByPID(uint64_t pid);
static void lockCurrentProcess(sosem_t *sem);
#endif

/* ------------------------------ */

sosem_t *sosem_open(const char *name, unsigned int initial_value)
{
        sosem_t *sem = NULL;

        int index = get_semaphore_index_from_table(name);
        if (index == -1) {
                create_named_semaphore(name, initial_value, &sem);
                add_semaphore_to_table(&sem);
        } else {
                sem = sosem_names_table[index].v;
        }

        return sem;
}

/* ------------------------------ */

int sosem_close(sosem_t *sem)
{
        if (sem == NULL)
                return -1;

        int idx = get_semaphore_index_from_table(sem->name);

        if (idx == -1)
                return -1;

        // Free all waiting processes (if any)
        atomic_store(&sem->value, SEM_MAX_VALUE);

        uint64_t pid = 0;
        while (pid_queue_shift(sem, &pid) == 0) {
                unlockProcessByPID(pid);
        }

        userland_destroy(sem);
        remove_semaphore_from_table(idx);

        return 0;
}

/* ------------------------------ */

int sosem_init(sosem_t *sem, unsigned int initial_value)
{
        if (sem == NULL)
                return -1;

        sem->name[0] = '\0';
        sem->binary = 0;

        atomic_store(&sem->value, initial_value);
        atomic_flag_clear(&sem->lock);
        atomic_store(&sem->_n_waiting, 0);

        userland_init(sem, &sem->userland);
        pid_queue_init(sem);

        return 0;
}

/* ------------------------------ */

int sosem_init_bin(sosem_t *sem, unsigned int initial_value)
{
        if (sem == NULL)
                return -1;

        sosem_init(sem, initial_value);
        sem->binary = 1;
        if (initial_value > 1)
                initial_value = 1;

        return 0;
}

/* ------------------------------ */

int sosem_destroy(sosem_t *sem)
{
        if (sem == NULL)
                return -1;

        // Free all waiting processes (if any)
        atomic_store(&sem->value, SEM_MAX_VALUE);

        uint64_t pid = 0;
        while (pid_queue_shift(sem, &pid) == 0) {
                unlockProcessByPID(pid);
        }



        userland_destroy(sem);

        return 0;
}

/* ------------------------------ */

int sosem_post(sosem_t *sem)
{
        if (sem == NULL)
                return -1;

        // Acquiring the lock (sem->lock) can result in a deadlock if
        // another process is in the middle of waiting. Fortunately,
        // the "worst case sceneario" is a single process busy waiting
        // if a context swich happens right after unlockProcessByPID().
        // The scheduler will waste that quantum and eventualy perform the
        // atomic_fetch_add and allow the locked process to go on.
        uint64_t pid = 0;
        if (pid_queue_shift(sem, &pid) == 0) {
                unlockProcessByPID(pid);
        }
        if (sem->binary == 1)
                atomic_store(&sem->value, 1);
        else
                atomic_fetch_add(&sem->value, 1);

        if (try_acquire(&(sem->lock)) == 0) {
                // Userland
                userland_update(sem);
                release(&(sem->lock));
        }

        return 0;
}

/* ------------------------------ */

int sosem_wait(sosem_t *sem)
{
        if (sem == NULL)
                return -1;

        // We are guaranteed that atomic_* functions are atomic, but we do not
        // have any guarantees about them being executed atomically as a block
        // Plus we have to update the Userland info (which is not atomic)
        acquire(&(sem->lock));

        atomic_fetch_add(&sem->_n_waiting, 1);

        // Userland
        while (atomic_load(&sem->value) == 0) {
                pid_queue_push(sem, getCurrentProcessPID());
                userland_update(sem);

                release(&(sem->lock));
#ifdef TESTING
                lockCurrentProcess(sem);
#else
                lockCurrentProcess();
#endif
        }

        // In case the current process was locked
        // Not needed in any other case
        try_acquire(&(sem->lock));

        atomic_fetch_sub(&sem->value, 1);
        atomic_fetch_sub(&sem->_n_waiting, 1);

        // Userland
        userland_update(sem);

        release(&(sem->lock));

        return 0;
}

/* ------------------------------ */

sosem_info_t *sosem_getinformation(sosem_info_t *restrict last)
{
        if (list_sosem_head == NULL)
                return NULL;
        else if (last == NULL)
                return &list_sosem_head->sem->userland;

        sosem_node_t *c = list_sosem_head;
        while (&c->sem->userland != last) {
                c = c->next;

                if (c == NULL)
                        return NULL;
        }
        if (c->next == NULL)
                return NULL;

        return &c->next->sem->userland;
}

/* ------------------------------ */

int sosem_getvalue(sosem_t *restrict sem, unsigned int *restrict sval)
{
        if (sem == NULL || sval == NULL)
                return -1;

        if (atomic_load(&sem->_n_waiting) > 0)
                *sval = 0;
        else
                *sval = atomic_load(&sem->value);

        return 0;
}

/* ------------------------------ */

static int get_semaphore_index_from_table(const char *name)
{
        if (name == NULL)
                return -1;

        uint64_t hash = djb2((unsigned char *)name);
        int idx = 0;

        for (int i = 0; i < SEM_MAX_NAMED; i++) {
                idx = i + hash % SEM_MAX_NAMED;
                // Avoid possible array overrun
                if (idx >= SEM_MAX_NAMED) {
                        idx -= SEM_MAX_NAMED;
                }

                if (sosem_names_table[idx].k == hash)
                        break;
                else
                        idx = -1;
        }

        return idx;
}

/* ------------------------------ */

static int add_semaphore_to_table(sosem_t **sem)
{
        if (sem == NULL)
                return -1;

        uint64_t hash = djb2((unsigned char *)(*sem)->name);
        int idx = 0;

        for (int i = 0; i < SEM_MAX_NAMED; i++) {
                idx = i + hash % SEM_MAX_NAMED;
                // Avoid possible array overrun
                if (idx >= SEM_MAX_NAMED) {
                        idx -= SEM_MAX_NAMED;
                }

                if (sosem_names_table[idx].k == 0) {
                        sosem_names_table[idx].k = hash;
                        sosem_names_table[idx].v = *sem;

                        break;
                } else {
                        idx = -1;
                }
        }

        return (idx >= 0 ? 0 : -1);
}

/* ------------------------------ */

static int remove_semaphore_from_table(unsigned int index)
{
        if (index >= SEM_MAX_NAMED)
                return -1;

        sosem_names_table[index].k = 0;
        sofree(sosem_names_table[index].v);
        sosem_names_table[index].v = NULL;

        return 0;
}

/* ------------------------------ */

static int create_named_semaphore(const char *name, unsigned int initial_value,
                                  sosem_t **sem)
{
        if (name == NULL || sem == NULL)
                return -1;

        *sem = somalloc(sizeof(sosem_t));
        if (*sem == NULL)
                return -1;

        size_t len = strnlen(name, SEM_MAX_NAME_LEN);
        somemcpy(&(*sem)->name, name, len);
        (*sem)->name[len] = '\0';

        (*sem)->binary = 0;

        atomic_store(&(*sem)->value, initial_value);
        atomic_flag_clear(&(*sem)->lock);
        atomic_store(&(*sem)->_n_waiting, 0);

        userland_init(*sem, &(*sem)->userland);
        pid_queue_init(*sem);

        return 0;
}

/* ------------------------------ */

static inline void userland_init(sosem_t *restrict sem,
                                 sosem_info_t *restrict info)
{
        info->name = &sem->name[0];
        info->len = strnlen(sem->name, SEM_MAX_NAME_LEN);

        info->value = atomic_load(&sem->value);

        info->waiting_pid = somalloc(SEM_MAX_WAITING * sizeof(uint64_t));
        info->n_waiting = sem->_processes._n_waiting;

        list_sosem_push(sem);
}

/* ------------------------------ */

static inline void userland_destroy(sosem_t *restrict sem)
{
        sem->userland.name = NULL;
        sem->userland.len = 0;

        sem->userland.value = 0;
        sem->userland.n_waiting = sem->_processes._n_waiting;

        sofree(sem->userland.waiting_pid);
        sem->userland.waiting_pid = NULL;

        list_sosem_remove(sem);
}

/* ------------------------------ */

static inline void userland_update(sosem_t *restrict sem)
{
        sem->userland.value = atomic_load(&sem->value);
        sem->userland.n_waiting = sem->_processes._n_waiting;

        for (unsigned int i = 0; i < sem->_processes._n_waiting;
             i = (i + 1) % SEM_MAX_WAITING) {
                unsigned int queue_index =
                        (i + sem->_processes._index) % SEM_MAX_WAITING;
                sem->userland.waiting_pid[i] =
                        sem->_processes._queue[queue_index];
        }
}

/* ------------------------------ */

static inline void pid_queue_init(sosem_t *restrict sem)
{
        if (sem == NULL)
                return;

        sem->_processes._n_waiting = 0;
        sem->_processes._queue[0] = 0;
        sem->_processes._index = 0;
}

/* ------------------------------ */

static inline int pid_queue_shift(sosem_t *restrict sem, uint64_t *pid)
{
        if (sem == NULL || pid == NULL)
                return -1;

        if (sem->_processes._n_waiting == 0)
                return -1;

        *pid = sem->_processes._queue[sem->_processes._index];
        sem->_processes._queue[sem->_processes._index] = 0;
        sem->_processes._index = (sem->_processes._index + 1) % SEM_MAX_WAITING;
        sem->_processes._n_waiting--;

        return 0;
}

/* ------------------------------ */

static inline int pid_queue_push(sosem_t *restrict sem, uint64_t pid)
{
        if (sem == NULL)
                return -1;

        if (sem->_processes._n_waiting == SEM_MAX_WAITING)
                return -1;

        unsigned int index =
                (sem->_processes._index + sem->_processes._n_waiting) %
                SEM_MAX_WAITING;
        sem->_processes._queue[index] = pid;
        sem->_processes._n_waiting++;

        return 0;
}

/* ------------------------------ */

static inline int list_sosem_push(sosem_t *restrict sem)
{
        if (sem == NULL)
                return -1;

        if (list_sosem_head == NULL) {
                list_sosem_head = somalloc(sizeof(sosem_node_t));
                if (list_sosem_head == NULL)
                        return -1;

                list_sosem_head->prev = NULL;
                list_sosem_tail = list_sosem_head;
        } else {
                list_sosem_tail->next = somalloc(sizeof(sosem_node_t));
                if (list_sosem_tail->next == NULL)
                        return -1;

                list_sosem_tail->next->prev = list_sosem_tail;
                list_sosem_tail = list_sosem_tail->next;
        }

        list_sosem_tail->sem = sem;
        list_sosem_tail->next = NULL;

        return 0;
}

/* ------------------------------ */

static inline int list_sosem_remove(sosem_t *restrict sem)
{
        if (sem == NULL)
                return -1;
        if (list_sosem_head == NULL)
                return 0;

        sosem_node_t *c = list_sosem_head;
        while (c != NULL && c->sem != sem) {
                c = c->next;
        }
        if (c == NULL)
                return 0;

        sosem_node_t *p = c->prev;
        sosem_node_t *n = c->next;

        if (p != NULL) // Current (c) could be head
                p->next = n;

        if (n != NULL) // Current (c) could be tail
                n->prev = p;

        if (p == NULL && n == NULL && c == list_sosem_head) {
                list_sosem_head = NULL;
                list_sosem_tail = NULL;
        } else if (n == NULL && c == list_sosem_tail) {
                list_sosem_tail = p;
        }

        sofree(c);

        return 0;
}

/* ------------------------------ */

#ifdef TESTING
static const uint64_t pids[SEM_MAX_WAITING + 1] = {
        14166, 43263, 17495, 62492, 54659, 40748, 30951, 41324, 4010,
        35957, 38284, 36361, 16601, 41779, 6083,  50030, 9082,  51745,
        26835, 2667,  47584, 44811, 41490, 15464, 29675, 26681, 23063,
        38607, 65432, 64108, 40418, 6242,  5291
};
static unsigned int last_pid = 0;

static uint64_t getCurrentProcessPID()
{
        uint64_t pid = pids[last_pid];
        last_pid = (last_pid + 1) % (SEM_MAX_WAITING + 1);
        return pid;
}

static void unlockProcessByPID(uint64_t pid)
{
        return;
}

static void lockCurrentProcess(sosem_t *sem)
{
        while (atomic_load(&sem->value) == 0)
                ;
}
#endif

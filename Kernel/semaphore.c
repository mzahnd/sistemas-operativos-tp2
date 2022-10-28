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
 * In case you do not have the manual entry for stdatomic.h is available online
 * https://en.cppreference.com/w/c/atomic
 */
#include "lib.h"
#include "mem/memory.h"

#include "semaphore.h"

/* ------------------------------ */

#define SEM_INITIAL_CHAR '$'

#define acquire(lock) while (atomic_flag_test_and_set(lock))
#define release(lock) atomic_flag_clear(lock)

/* ------------------------------ */

typedef struct {
        uint64_t k; // hash
        sosem_t *v; // semaphore
} sosem_kv_t;

static sosem_kv_t sosem_names_table[SEM_MAX_NAMED] = {};

/* ------------------------------ */

// static int get_semaphore_index_from_table(const char *name);
static int add_semaphore_to_table(sosem_t **sem);
static int remove_semaphore_from_table(unsigned int index);
static int create_named_semaphore(const char *name, unsigned int initial_value,
                                  sosem_t **sem);

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
                return 0;

        // Free all waiting processes (if any)
        if (sosem_names_table[idx].v)
                atomic_store(&(sosem_names_table[idx].v->value), SEM_MAX_VALUE);

        return remove_semaphore_from_table(idx);
}

/* ------------------------------ */

int sosem_init(sosem_t *sem, unsigned int initial_value)
{
        if (sem == NULL)
                return -1;

        atomic_store(&sem->value, initial_value);
        atomic_flag_clear(&sem->lock);
        atomic_store(&sem->_n_waiting, 0);

        return 0;
}

/* ------------------------------ */

int sosem_destroy(sosem_t *sem)
{
        if (sem == NULL)
                return -1;

        // Free all waiting processes (if any)
        atomic_store(&sem->value, SEM_MAX_VALUE);
        return 0;
}

/* ------------------------------ */

int sosem_post(sosem_t *sem)
{
        if (sem == NULL)
                return -1;

        return atomic_fetch_add(&sem->value, 1);
}

/* ------------------------------ */

int sosem_wait(sosem_t *sem)
{
        if (sem == NULL)
                return -1;

        // We are guaranteed that atomic_* functions are atomic, but we do not
        // have any guarantees about them being executed atomically as a block
        acquire(&(sem->lock));
        atomic_fetch_add(&sem->_n_waiting, 1);

        while (atomic_load(&sem->value) == 0)
                ;
        atomic_fetch_sub(&sem->value, 1);

        atomic_fetch_sub(&sem->_n_waiting, 1);
        release(&(sem->lock));

        return 0;
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

int get_semaphore_index_from_table(const char *name)
{
        if (name == NULL)
                return -1;

        uint64_t hash = djb2((unsigned char *)name);
        int idx = 0;

        for (int i = 0; i < SEM_MAX_NAMED; i++) {
                idx = i + hash % SEM_MAX_NAMED;

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

                if (sosem_names_table[idx].k == 0) {
                        sosem_names_table[idx].k = hash;
                        sosem_names_table[idx].v = *sem;

                        break;
                } else {
                        idx = -1;
                }
        }

        return idx >= 0;
}

/* ------------------------------ */

static int remove_semaphore_from_table(unsigned int index)
{
        if (index > SEM_MAX_NAMED)
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

        atomic_store(&(*sem)->value, initial_value);
        atomic_flag_clear(&(*sem)->lock);
        atomic_store(&(*sem)->_n_waiting, 0);

        return 0;
}

#ifndef SEMAPHORE_USER_H
#define SEMAPHORE_USER_H

#include <stdint.h>
#include <stdio.h>
#include <stdatomic.h> /* atomic_* */

/*
 * Maximum value a semaphore can take.
 */
#define SEM_MAX_VALUE (~((unsigned int)0))

#define SEM_MAX_NAME_LEN ((2 << 7) - 1) // 254

/**
 * Maximum number of named semaphores
 */
#define SEM_MAX_NAMED (2 << 4) // 256

// Information for Userland.
typedef struct SOSEM_INFO {
        char *name;
        size_t len;

        unsigned int value;

        uint64_t *waiting_pid;
        size_t n_waiting;
} sem_info_t;

typedef struct SEM {
        char name[SEM_MAX_NAME_LEN + 1];

        atomic_uint value;
        atomic_flag lock;
        atomic_uint _n_waiting;

        sem_info_t userland;
} sem_t;

// Create a named semaphore starting with initial_value.
sem_t *sosem_open(const char *name, unsigned int initial_value);
// Close a named semaphore
int sem_close(sem_t *sem);
// Create an unnamed semaphore starting with initial_value.
int sem_init(sem_t *sem, unsigned int initial_value);
// Close an unnamed semaphore
int sem_destroy(sem_t *sem);
int sem_getvalue(sem_t *restrict sem, unsigned int *restrict sval);
int sem_post(sem_t *sem);
int sem_wait(sem_t *sem);

sem_info_t *sem_getinformation(sem_t *restrict sem);

#endif

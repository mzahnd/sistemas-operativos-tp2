// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**
 * This file is part of sistemas-operativos-tp2
 * Licensed under BSD 3-Clause "New" or "Revised" License.
 * Copyright (c) 2022 Flores Levalle, M.
 *                    López, P.
 *                    Sierra Pérez, C.
 *                    Zahnd, M. E.
 */
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdatomic.h> /* atomic_* */
#include <stdint.h> /* int64_t */
#include <stddef.h> /* size_t */

/*
 * Maximum value a semaphore can take.
 */
#define SEM_MAX_VALUE (~((unsigned int)0))

#define SEM_MAX_NAME_LEN ((2 << 7) - 1) // 254

/**
 * Maximum number of named semaphores
 */
#define SEM_MAX_NAMED (2 << 4) // 32

#define SEM_MAX_WAITING (2 << 4) // 32

// Information for Userland.
typedef struct SOSEM_INFO {
        char *name;
        size_t len;

        unsigned int value;

        uint64_t *waiting_pid;
        size_t n_waiting;
} sosem_info_t;

// Circular queue with locked processes' PID
typedef struct _SOSEM_PID {
        unsigned int _n_waiting;
        uint64_t _queue[SEM_MAX_WAITING];
        unsigned int _index;
} _sosem_pid_t;

typedef struct SOSEM {
        char name[SEM_MAX_NAME_LEN + 1];

        atomic_uint value;
        atomic_flag lock;
        atomic_uint _n_waiting;

        sosem_info_t userland;

        _sosem_pid_t _processes;
} sosem_t;

// Create a named semaphore starting with initial_value.
sosem_t *sosem_open(const char *name, unsigned int initial_value);
// Close a named semaphore
int sosem_close(sosem_t *sem);
// Create an unnamed semaphore starting with initial_value.
int sosem_init(sosem_t *sem, unsigned int initial_value);
// Close an unnamed semaphore
int sosem_destroy(sosem_t *sem);
int sosem_getvalue(sosem_t *restrict sem, unsigned int *restrict sval);
int sosem_post(sosem_t *sem);
int sosem_wait(sosem_t *sem);

sosem_info_t *sosem_getinformation(sosem_info_t *restrict last);

#endif /* SEMAPHORE_H */

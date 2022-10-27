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

/*
 * Maximum value a semaphore can take.
 */
#define SEM_MAX_VALUE (~((unsigned int)0))

#define SEM_MAX_NAME_LEN ((2 << 7) - 1) // 254

/**
 * Maximum number of named semaphores
 */
#define SEM_MAX_NAMED (2 << 4) // 256

typedef struct SOSEM {
        char name[SEM_MAX_NAME_LEN + 2];

        atomic_uint value;
        atomic_flag lock;

        atomic_uint _n_waiting;
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

#endif /* SEMAPHORE_H */

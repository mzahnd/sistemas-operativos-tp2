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

#include <stdint.h> /* int64_t */

#define SOSEM_NAME_MAX ((2 << 7) - 2)

typedef struct SOSEM {
        int64_t value;
        char name[SOSEM_NAME_MAX + 2];
} sosem_t;

int sosem_close(sosem_t *sem);
sosem_t *sosem_open(const char *name, uint64_t initial_value);
int sosem_post(sosem_t *sem);
int sosem_wait(sosem_t *sem);

#endif /* SEMAPHORE_H */

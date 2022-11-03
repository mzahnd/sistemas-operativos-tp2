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
#ifndef SEMAPHORE_USER_H
#define SEMAPHORE_USER_H

#include <syscalls_definitions.h> /* sem_t, sem_info_t, SEM_MAX_NAME_LEN, 
                                   * SEM_MAX_WAITING*/

/*
 * Maximum value a semaphore can take.
 */
#define SEM_MAX_VALUE (~((unsigned int)0))

#define SEM_MAX_NAME_LEN ((2 << 7) - 1) // 254

/**
 * Maximum number of named semaphores
 */
#define SEM_MAX_NAMED (2 << 4) // 32

// Create a named semaphore starting with initial_value.
sem_t *sem_open(const char *name, unsigned int initial_value);
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

#endif /* SEMAPHORE_USER_H */

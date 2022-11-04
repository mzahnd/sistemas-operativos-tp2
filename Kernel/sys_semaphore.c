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
 * This source code has been insipred in FreeRTOS heap_4 and K&R malloc.
 *
 * See: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/portable/MemMang/heap_4.c
 */
#include <sys_semaphore.h>

inline void sys_sosem_open(const char *name, unsigned int initial_value,
                           sosem_t **result)
{
        (*result) = sosem_open(name, initial_value);
}

/* ------------------------------ */

inline void sys_sosem_close(sosem_t *sem, int *result)
{
        *result = sosem_close(sem);
}

/* ------------------------------ */

inline void sys_sosem_init(sosem_t *sem, unsigned int initial_value,
                           int *result)
{
        *result = sosem_init(sem, initial_value);
}

/* ------------------------------ */

inline void sys_sosem_destroy(sosem_t *sem, int *result)
{
        *result = sosem_destroy(sem);
}

/* ------------------------------ */

inline void sys_sosem_getvalue(sosem_t *restrict sem,
                               unsigned int *restrict sval, int *result)
{
        *result = sosem_getvalue(sem, sval);
}

/* ------------------------------ */

inline void sys_sosem_post(sosem_t *sem, int *result)
{
        *result = sosem_post(sem);
}

/* ------------------------------ */

inline void sys_sosem_wait(sosem_t *sem, int *result)
{
        *result = sosem_wait(sem);
}

/* ------------------------------ */

inline void sys_sosem_getinformation(sosem_t *sem, sosem_info_t **result)
{
        *result = sosem_getinformation(sem);
}

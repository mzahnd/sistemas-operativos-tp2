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
#include <mem/memory.h>
#include <mem/sys_memory.h>

inline void sys_somalloc(size_t size, void **result)
{
        (*result) = somalloc(size);
}

/* ------------------------------ */

inline void sys_socalloc(size_t nmemb, size_t size, void **result)
{
        (*result) = socalloc(nmemb, size);
}

/* ------------------------------ */

inline void sys_sofree(void *ptr)
{
        sofree(ptr);
}

/* ------------------------------ */

void sys_somem_getinformation(somem_info_t **result)
{
        (*result) = somem_getinformation();
}

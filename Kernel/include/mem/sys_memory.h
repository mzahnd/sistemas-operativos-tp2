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
#ifndef SYS_MEMORY_H
#define SYS_MEMORY_H

#include <stddef.h> /* size_t */
#include <mem/memory.h> /* somem_info_t */

/** 
 * Wrapper around Kernel's somalloc() for Userland.
 *
 * @see Kernel's function: Kernel/include/mem/memory.h 
 *
 * @param size Requested memory size
 * @param result Address of requested memory
 */
void sys_somalloc(size_t size, void **result);

/**
 * Wrapper around Kernel's socalloc() for Userland.
 *
 * @see Kernel's function: Kernel/include/mem/memory.h 
 *
 * @param nmemb Requested number of memory blocks
 * @param size Size of each memory block
 * @param result Address of requested memory
 */
void sys_socalloc(size_t nmemb, size_t size, void **result);

/**
 * Wrapper around Kernel's sofree() for Userland.
 *
 * @see Kernel's function: Kernel/include/mem/memory.h 
 *
 * @param ptr Address of memory block returned by sys_somalloc() or
 *            sys_socalloc()
 */
void sys_sofree(void *ptr);

/**
 * Wrapper around Kernel's somem_getinformation() for Userland.
 *
 * @see Kernel's function: Kernel/include/mem/memory.h 
 *
 * @param result Address of memory information structure
 */
void sys_somem_getinformation(somem_info_t **result);

#endif /* SYS_MEMORY_H */

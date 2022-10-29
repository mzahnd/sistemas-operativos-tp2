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
#ifndef SYS_SEMAPHORE_H
#define SYS_SEMAPHORE_H

#include "semaphore.h"

/** 
 * Wrapper around Kernel's sosem_open() for Userland.
 *
 * @see Kernel's function: Kernel/include/semaphore.h 
 *
 * @param name Semaphore name
 * @param initial_value Iniital value
 * @param result Address of requested semaphore
 */
void sys_sosem_open(const char *name, unsigned int initial_value,
                    sosem_t **result);

/** 
 * Wrapper around Kernel's sys_sosem_close() for Userland.
 *
 * @see Kernel's function: Kernel/include/semaphore.h 
 *
 * @param sem Address of the named semaphore to close
 * @param result 0 on success; -1 otherwise
 */
void sys_sosem_close(sosem_t *sem, int *result);

/** 
 * Wrapper around Kernel's sys_sosem_init() for Userland.
 *
 * @see Kernel's function: Kernel/include/semaphore.h 
 *
 * @param sem Address of the unnamed semaphore to iniitlize
 * @param result 0 on success; -1 otherwise
 */
void sys_sosem_init(sosem_t *sem, unsigned int initial_value, int *result);

/** 
 * Wrapper around Kernel's sys_sosem_destroy() for Userland.
 *
 * @see Kernel's function: Kernel/include/semaphore.h 
 *
 * @param sem Address of the unnamed semaphore to destroy
 * @param result 0 on success; -1 otherwise
 */
void sys_sosem_destroy(sosem_t *sem, int *result);

/** 
 * Wrapper around Kernel's sys_sosem_getvalue() for Userland.
 *
 * @see Kernel's function: Kernel/include/semaphore.h 
 *
 * @param sem Address of the semaphore to query
 * @param sval Semaphores' value
 * @param result 0 on success; -1 otherwise
 */
void sys_sosem_getvalue(sosem_t *restrict sem, unsigned int *restrict sval,
                        int *result);

/** 
 * Wrapper around Kernel's sys_sosem_post() for Userland.
 *
 * @see Kernel's function: Kernel/include/semaphore.h 
 *
 * @param sem Address of the unnamed semaphore to post
 * @param result 0 on success; -1 otherwise
 */
void sys_sosem_post(sosem_t *sem, int *result);

/** 
 * Wrapper around Kernel's sys_sosem_wait() for Userland.
 *
 * @see Kernel's function: Kernel/include/semaphore.h 
 *
 * @param sem Address of the unnamed semaphore to wait
 * @param result 0 on success; -1 otherwise
 */
void sys_sosem_wait(sosem_t *sem, int *result);

/** 
 * Wrapper around Kernel's sosem_getinformation() for Userland.
 *
 * @see Kernel's function: Kernel/include/semaphore.h 
 *
 * @param sem Address of the unnamed semaphore to wait
 * @param result 0 on success; -1 otherwise
 */
void sys_sosem_getinformation(sosem_t *sem, sosem_info_t **result);

#endif /* SYS_SEMAPHORE_H */

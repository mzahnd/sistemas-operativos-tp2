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
#ifndef SYS_PIPES_H
#define SYS_PIPES_H

#include "pipes.h"

/** 
 * Wrapper around Kernel's sopipe() for Userland.
 *
 * @see Kernel's function: Kernel/include/pipes.h 
 *
 * @param fildes Array to store the assigned file descriptors
 * @param result 0 on success, -1 otherwise
 */
void sys_sopipe(int fildes[PIPE_N_FD], int *result);

/** 
 * Wrapper around Kernel's soread() for Userland.
 *
 * @see Kernel's function: Kernel/include/pipes.h 
 *
 * @param fd File descriptor to read from
 * @param buf Start position of the buffer to store read bytes
 * @param count Maximum number of bytes to write un buf
 * @param result Number of bytes actually read from the fd and stored in buf
 */
void sys_soread(int fd, char *buf, size_t count, ssize_t *result);

/** 
 * Wrapper around Kernel's sowrite() for Userland.
 *
 * @see Kernel's function: Kernel/include/pipes.h 
 *
 * @param fd File descriptor to write
 * @param buf Start position of the buffer to read characters from
 * @param count Maximum number of bytes to read from buf
 * @param result Number of bytes actually written in fd
 */
void sys_sowrite(int fd, const char *buf, size_t count, ssize_t *result);

/** 
 * Wrapper around Kernel's soclose() for Userland.
 *
 * @see Kernel's function: Kernel/include/pipes.h 
 *
 * @param fd File descriptor to close. If its the last from a pipe, also closes
 * the corresponding pipe.
 * @param result 0 on success, -1 otherwise
 */
void sys_soclose(int fd, int *result);

/** 
 * Wrapper around Kernel's soclose() for Userland.
 *
 * @see Kernel's function: Kernel/include/pipes.h 
 *
 * @param fd File descriptor of a pipe
 * @param result Structure with information about the pipe that contains fd
 */
void sys_sopipe_getinformation(sopipe_info_t * last, sopipe_info_t **result);

#endif /* SYS_PIPES_H */

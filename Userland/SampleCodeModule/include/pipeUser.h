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
#ifndef PIPE_USER_H
#define PIPE_USER_H

#include <sys/types.h> /* ssize_t */

enum pipe_fd { PIPE_FD_READ = 0, PIPE_FD_WRITE, PIPE_N_FD };

typedef struct {
        const char *buffer;
        size_t size;

        int fd[PIPE_N_FD];
        int active;
} pipe_info_t;

int pipe(int fildes[PIPE_N_FD]);
int close(int fd);

#endif /* PIPE_USER_H */

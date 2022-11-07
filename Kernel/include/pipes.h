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
#ifndef PIPES_H
#define PIPES_H

#include <sys/types.h> /* ssize_t */

#define PIPE_BUFFER_SIZE 4096
#define MAX_PIPES 32

enum pipe_fd { PIPE_FD_READ = 0, PIPE_FD_WRITE, PIPE_N_FD };

// Information for Userland.
#define USERLAND_FD_CLOSED -1
typedef struct {
        const char *buffer;
        size_t size;

        int fd[PIPE_N_FD]; // One element could be USERLAND_FD_CLOSED
} sopipe_info_t;

int sopipe(int fildes[PIPE_N_FD]);
ssize_t soread(int fd, char *buf, size_t count);
ssize_t sowrite(int fd, const char *buf, size_t count);
int soclose(int fd);

/* Return another pipe information.
 *
 * When last is NULL it returns the oldest pipe information.
 *
 */
sopipe_info_t *sopipe_getinformation(sopipe_info_t *restrict last);

#endif /* PIPES_H */

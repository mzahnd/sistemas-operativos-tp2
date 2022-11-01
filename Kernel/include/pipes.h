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
typedef struct {
        const char *buffer;
        size_t size;

        int fd[PIPE_N_FD];
        int active;
} pipe_info_t;

int sopipe(int fildes[PIPE_N_FD]);
ssize_t soread(int fd, char *buf, size_t count);
ssize_t sowrite(int fd, const char *buf, size_t count);
int soclose(int fd);

// Given a fd, look up for the pipe that contains it and return information
// about it. Usefull in Userland.
// Returns NULL if the file descriptor is not associated with any pipe.
pipe_info_t *sopipe_getinformation(int fd);

#endif /* PIPES_H */

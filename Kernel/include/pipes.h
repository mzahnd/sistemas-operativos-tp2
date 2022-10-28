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
#include <stdint.h>

#define PIPE_BUFFER_SIZE 4096
#define MAX_PIPES 10

enum pipe_fd { PIPE_FD_READ = 0, PIPE_FD_WRITE, PIPE_N_FD };

typedef struct Pipe{
	int id;
    char *pipeBuffer;
    int readIndex;
    int writeIndex;
    int processCount;               //Amount of processes using the pipe
	uint32_t readSem;               //Semaphore for pipe read (non binary)
	uint32_t writeSem;              //Semaphore for pipe write (non binary)
    unsigned int fdIn;
    unsigned int fdOut;
}Pipe;

int sopipe(int fildes[PIPE_N_FD]);
ssize_t soread(int fd, char *buf, size_t count);
ssize_t sowrite(int fd, const char *buf, size_t count);
int soclose(int fd);

#endif /* PIPES_H */

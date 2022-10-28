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
 *
 * This implementation is based on Michael Cotterell (@mepcotterell) Gist's
 * on semaphores.
 * See: https://gist.github.com/mepcotterell/6f0a779befe388ab822764255e3776ae
 *
 * In case you do not have the manual entry for stdatomic.h is available online
 * https://en.cppreference.com/w/c/atomic
 */
#include "lib.h"
#include "include/semaphore.h"

#include "include/pipes.h"
#define MAX_READ 50

/* ------------------------------ */
Pipe pipes[MAX_PIPES];
int openedPipes = 0;
/* ------------------------------ */


static void createPipe(int id) {

    Pipe *pipe = &pipes[id];
    pipe->id = id;

    sosem_t *sem = sosem_open("nextSemaphore", 0);

    int nextSemaphore = get_semaphore_index_from_table("nextSemaphore");

    pipe -> readSem = nextSemaphore++;
    pipe -> writeSem = nextSemaphore++;

}

static int searchPipe(int fd) {
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipes[i].fdIn == fd || pipes[i].fdOut == fd) {
            return i;
        }
    }
    return -1;
}


int sopipe(int fildes[PIPE_N_FD]) {
    if (openedPipes == MAX_PIPES - 1) {
        return -1;
    }

    for(int i = 0; i < MAX_PIPES; i++) {
        if(pipes[i].processCount == 0) {
            createPipe(i);
            pipes[i].processCount++;
            fildes[0] = pipes[i].fdIn;
            fildes[1] = pipes[i].fdOut;
            pipes[i].readIndex = 0;
            pipes[i].writeIndex = 0;
            openedPipes++;
            return 1;
        }
    }
}

ssize_t soread(int fd, char *buf, size_t count) {
    int index = searchPipe(fd);
    if (index == -1 || pipes[index].fdOut == fd || count <= 0) {
        return 0;
    }

    Pipe auxPipe = pipes[index];

    sosem_wait(auxPipe.readSem);
   
    int i;
    for (i = 0; auxPipe.readIndex != auxPipe.writeIndex && i < count; i++) {
        buf[i] = auxPipe.pipeBuffer[auxPipe.readIndex];
        auxPipe.pipeBuffer[auxPipe.readIndex] = 0;
        auxPipe.readIndex = (auxPipe.readIndex + 1) % PIPE_BUFFER_SIZE;
    }

    sosem_post(auxPipe.writeSem);

    buf[i] = 0;
    return i;
}

ssize_t sowrite(int fd, const char *buf, size_t count) {
    int index = searchPipe(fd);
    if (index == -1 || pipes[index].fdIn == fd || count <= 0) {
        return 0;
    }

    Pipe auxPipe = pipes[index];

    sosem_wait(auxPipe.writeSem);

    int i;
    for (i = 0; i < count; i++) {
        auxPipe.pipeBuffer[auxPipe.writeIndex] = buf[i];
        auxPipe.writeIndex = (auxPipe.writeIndex + 1) % PIPE_BUFFER_SIZE;
    }

    sosem_post(auxPipe.readSem);

    
    return i;
}

int soclose(int fd) {
    int index = searchPipe(fd);

    Pipe auxPipe = pipes[index];

    sosem_close(auxPipe.readSem);
    sosem_close(auxPipe.writeSem);

    return 1;
    
}

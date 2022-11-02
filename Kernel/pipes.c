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
#include <lib.h>
#include <semaphore.h>

#include <pipes.h>

/* ------------------------------ */

#define FIRST_FD 3 // 0 = stdin; 1 = stdout; 2 = stderr

#define NOT_ASSIGNED 0
#define ASSIGNED 1

/* ------------------------------ */

typedef struct {
        sosem_t sem;
        int index;
} pipe_flow_t;

typedef struct {
        int active;

        char buffer[PIPE_BUFFER_SIZE];

        int fd[PIPE_N_FD];
        pipe_flow_t read;
        pipe_flow_t write;

        pipe_info_t userland;
} pipe_t;

/* ------------------------------ */

static int assigned_fd[MAX_PIPES * 2] = {}; // List of assigned file descriptors
static pipe_t pipes[MAX_PIPES] = {};
static int pipes_open = 0;

/* ------------------------------ */

static inline void createPipe(int index);
static inline int get_pipe_index(int fd);
static inline int get_fd_status(int fd);
static inline void store_fd_pair(int fd[PIPE_N_FD]);
static inline void dup_fd_pair(int dest[PIPE_N_FD], int src[PIPE_N_FD]);
static inline void remove_fd(int fd);
static inline int is_valid_fd(int fd);
static inline void userland_create(pipe_t *pipe, pipe_info_t *info);
static inline void userland_update(pipe_t *pipe);

/* ------------------------------ */

int sopipe(int fildes[PIPE_N_FD])
{
        if (pipes_open == MAX_PIPES) {
                return -1;
        }

        for (int i = 0; i < MAX_PIPES; i++) {
                if (pipes[i].active == 0) {
                        createPipe(i);
                        store_fd_pair(pipes[i].fd);
                        dup_fd_pair(fildes, pipes[i].fd);

                        pipes_open++;
                        return 0;
                }
        }

        return -1;
}

/* ------------------------------ */

ssize_t soread(int fd, char *buf, size_t count)
{
        if (count == 0 || buf == NULL || is_valid_fd(fd) == -1 ||
            assigned_fd[fd - FIRST_FD] == NOT_ASSIGNED)
                return 0;

        int index = get_pipe_index(fd);
        if (index == -1 || pipes[index].fd[PIPE_FD_WRITE] == fd) {
                return 0;
        }

        ssize_t n = 0;

        sosem_wait(&pipes[index].read.sem);

        int read_idx = pipes[index].read.index; // Alias for readabilty

        do {
                buf[n] = pipes[index].buffer[read_idx];
                pipes[index].buffer[read_idx] = 0;
                read_idx = (read_idx + 1) % PIPE_BUFFER_SIZE;

                n++;
        } while (read_idx != pipes[index].write.index && n - 1 < count);

        pipes[index].read.index = read_idx; // Update. Downside of the alias (:

        userland_update(&pipes[index]);

        sosem_post(&pipes[index].write.sem);

        return n;
}

/* ------------------------------ */

ssize_t sowrite(int fd, const char *buf, size_t count)
{
        if (count == 0 || buf == NULL || is_valid_fd(fd) == -1 ||
            assigned_fd[fd - FIRST_FD] == NOT_ASSIGNED)
                return 0;

        int index = get_pipe_index(fd);
        if (index == -1 || pipes[index].fd[PIPE_FD_READ] == fd) {
                return 0;
        }

        ssize_t n = 0;

        sosem_wait(&pipes[index].write.sem);

        for (n = 0; n < count && n < PIPE_BUFFER_SIZE; n++) {
                int write_idx =
                        pipes[index].write.index; // Alias for readabilty

                pipes[index].buffer[write_idx] = buf[n];
                pipes[index].write.index = (write_idx + 1) % PIPE_BUFFER_SIZE;
        }

        sosem_post(&pipes[index].read.sem);

        return n;
}

/* ------------------------------ */

int soclose(int fd)
{
        if (fd < FIRST_FD || fd > FIRST_FD + MAX_PIPES * 2)
                return -1;

        // Set fd as NOT_ASSIGNED
        remove_fd(fd);

        // Check if it's the last open fd from a pipe and close the pipe
        int index = get_pipe_index(fd);

        if (get_fd_status(pipes[index].fd[PIPE_FD_READ]) == NOT_ASSIGNED &&
            get_fd_status(pipes[index].fd[PIPE_FD_WRITE]) == NOT_ASSIGNED) {
                pipes[index].active = 0;
                pipes[index].buffer[0] = '\0';

                sosem_destroy(&pipes[index].read.sem);
                sosem_destroy(&pipes[index].write.sem);
        }

        return 0;
}

/* ------------------------------ */

pipe_info_t *sopipe_getinformation(int fd)
{
        int index = get_pipe_index(fd);
        if (index == -1)
                return NULL;

        return &pipes[index].userland;
}

/* ------------------------------ */

static inline void createPipe(int index)
{
        pipes[index].active = 1;
        somemset(pipes[index].buffer, '\0', PIPE_BUFFER_SIZE);

        pipes[index].read.index = 0;
        pipes[index].write.index = 0;

        sosem_init(&pipes[index].read.sem, 0);
        // Initial value > 0 to allow writting
        sosem_init(&pipes[index].write.sem, 1);

        userland_create(&pipes[index], &pipes[index].userland);
}

/* ------------------------------ */

static inline int get_pipe_index(int fd)
{
        for (int i = 0; i < MAX_PIPES; i++) {
                if (pipes[i].fd[PIPE_FD_READ] == fd ||
                    pipes[i].fd[PIPE_FD_WRITE] == fd) {
                        return i;
                }
        }

        return -1;
}

/* ------------------------------ */

static inline int get_fd_status(int fd)
{
        if (is_valid_fd(fd) == -1)
                return -1;

        return assigned_fd[fd - FIRST_FD];
}

/* ------------------------------ */

static inline void store_fd_pair(int fd[PIPE_N_FD])
{
        fd[PIPE_FD_READ] = -1;
        fd[PIPE_FD_WRITE] = -1;

        int rw = PIPE_FD_READ;
        for (int i = 0; i < MAX_PIPES * 2; i++) {
                if (assigned_fd[i] == NOT_ASSIGNED) {
                        fd[rw] = i + FIRST_FD;
                        assigned_fd[i] = ASSIGNED;

                        rw++;

                        if (rw != PIPE_FD_READ && rw != PIPE_FD_WRITE)
                                break;
                }
        }
}

/* ------------------------------ */

static inline void dup_fd_pair(int dest[PIPE_N_FD], int src[PIPE_N_FD])
{
        dest[PIPE_FD_READ] = src[PIPE_FD_READ];
        dest[PIPE_FD_WRITE] = src[PIPE_FD_WRITE];
}

/* ------------------------------ */

static inline void remove_fd(int fd)
{
        if (is_valid_fd(fd) == -1)
                return;

        assigned_fd[fd - FIRST_FD] = NOT_ASSIGNED;
}

/* ------------------------------ */

static inline int is_valid_fd(int fd)
{
        if (fd < FIRST_FD || fd > FIRST_FD + MAX_PIPES * 2)
                return -1;

        return 0;
}

/* ------------------------------ */

static inline void userland_create(pipe_t *pipe, pipe_info_t *info)
{
        info->buffer = &pipe->buffer[0];
        info->size = PIPE_BUFFER_SIZE;

        //Copy contents from pipe fd to info fd
        dup_fd_pair(info->fd, pipe->fd);

        info->active = pipe->active;
}

/* ------------------------------ */

static inline void userland_update(pipe_t *pipe)
{
        // In case someone rewrote it by mistake
        pipe->userland.buffer = &pipe->buffer[0];
        pipe->userland.size = PIPE_BUFFER_SIZE;

        pipe->userland.active = pipe->active;
}

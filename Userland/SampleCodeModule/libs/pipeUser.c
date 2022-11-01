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
#include <syscalls_asm.h>
#include <pipeUser.h>

int pipe(int fildes[PIPE_N_FD])
{
        int ret;
        pipePipeSyscall(fildes, &ret);
        return ret;
}

ssize_t read(int fd, char *buf, size_t count)
{
        ssize_t ret;
        pipeReadSyscall(fd, buf, count, &ret);
        return ret;
}

ssize_t write(int fd, const char *buf, size_t count)
{
        ssize_t ret;
        pipeWriteSyscall(fd, buf, count, &ret);
        return ret;
}

int close(int fd)
{
        int ret;
        pipeCloseSyscall(fd, &ret);
        return ret;
}

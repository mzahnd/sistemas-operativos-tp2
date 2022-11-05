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
#include <sys_pipes.h>

void sys_sopipe(int fildes[PIPE_N_FD], int *result)
{
        *result = sopipe(fildes);
}

void sys_soread(int fd, char *buf, size_t count, ssize_t *result)
{
        *result = soread(fd, buf, count);
}

void sys_sowrite(int fd, const char *buf, size_t count, ssize_t *result)
{
        *result = sowrite(fd, buf, count);
}

void sys_soclose(int fd, int *result)
{
        *result = soclose(fd);
}

void sys_sopipe_getinformation(sopipe_info_t *last, sopipe_info_t **result)
{
        *result = sopipe_getinformation(last);
}

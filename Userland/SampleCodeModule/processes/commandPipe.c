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
#include <stdio.h> /* printf() */
#include <stdlib.h> /* NULL */
#include <pipeUser.h> /* pipe_getinformation() */

int commandPipe(int argc, char **argv)
{
        unsigned n_pipes = 0;
        pipe_info_t *info = pipe_getinformation(NULL);
        printf("********** Pipes **********\n");
        while (info != NULL) {
                printf("Buffer: %s\n", info->buffer);
                printf("Buffer size: %d\n", info->size); // -V576

                printf("File descriptors:\n");
                printf("\tRead: ");
                if (info->fd[PIPE_FD_READ] == USERLAND_FD_CLOSED)
                        printf("[Closed]");
                else
                        printf("%d", info->fd[PIPE_FD_READ]);

                putChar('\n');

                printf("\tWrite: ");
                if (info->fd[PIPE_FD_WRITE] == USERLAND_FD_CLOSED)
                        printf("[Closed]");
                else
                        printf("%d", info->fd[PIPE_FD_WRITE]);

                putChar('\n');

                n_pipes++;
                info = pipe_getinformation(info);
                putChar('\n');
        }
        printf("***************************\n");
        printf("Total pipes: %d\n", n_pipes); // -V576

        return 0;
}

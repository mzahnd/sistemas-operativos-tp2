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
#include <semaphoreUser.h> /* sem_getinformation() */

int commandSem(int argc, char **argv)
{
        unsigned n_sem = 0;
        sem_info_t *info = sem_getinformation(NULL);
        printf("********** Semaphores **********\n");
        while (info != NULL) {
                printf("Name: ");
                if (info->name[0] == '\0')
                        printf("[Unnamed semaphore]\n");

                else
                        printf("%s\n", info->name);

                printf("Value: %d\n", info->value); // -V576
                printf("----- Waiting processes -----\n");
                printf("Ammount: %d\n", info->n_waiting); // -V576
                printf("PID: [");
                for (size_t i = 0; i < info->n_waiting; i++) {
                        printf("%d", info->waiting_pid[i]); // -V576

                        if (i < info->n_waiting - 1)
                                printf(", ");
                }
                printf("]\n");

                n_sem++;
                info = sem_getinformation(info);
                putChar('\n');
        }
        printf("********************************\n");
        printf("Total semaphores: %d\n", n_sem); // -V576

        return 0;
}

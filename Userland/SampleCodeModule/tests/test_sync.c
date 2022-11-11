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
 * Test provistos por la cátedra.
 */
#include <stdint.h>
#include <stdio.h>
#include <tests/test_util.h>
#include <stringUtils.h>
#include <stdlib.h> /* NULL */
#include <processManagement.h> /* createProcess(); */
#include <processes.h> /* commandBlock(); commandKill(); commandNice();
                        * commandUnblock(); */
#include <semaphoreUser.h> /* sem_open(); sem_wait(); sem_post(); 
                            * sem_close(); */

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 8

int64_t global = 0; //shared memory

void slowInc(int64_t *p, int64_t inc)
{
        int64_t aux = *p;
        // printf("B[%d] - ", getPid());
        giveUpCPU(); //This makes the race condition highly probable
        // printf("A [%d] - ", getPid());
        aux += inc;
        *p = aux;
        //printf("Ready [%d] by [%d]\n", getPid(), inc);
}

int my_process_inc(int argc, char *argv[])
{
        uint64_t n;
        int64_t inc;
        uint64_t use_sem;
        sem_t *sem = NULL;


        if (argc != 3) {
                printf("Error: Invalid number of arguments\n");
                return -1;
        }

        if ((n = satoi(argv[1])) <= 0) {
                printf("Error: Repetitions is not valid\n");
                return -1;
        }
        if (strcmp(argv[2], "1") == 0) {
                inc = 1;
        } else {
                inc = -1;
        }
        if ((use_sem = satoi(argv[3])) < 0) {
                printf("Error: Use sem is not valid\n");
                return -1;
        }

        if (use_sem) {
                sem = sem_open(SEM_ID, 1);
        }
        if (sem == NULL && use_sem) {
                printf("test_sync: ERROR opening semaphore\n");
                return -1;
        }
        // printf("Process [%d], n: [%d] - inc: [%s]\n", getPid(), n, inc == 1 ? "1" : "-1");

        uint64_t i;
        for (i = 0; i < n; i++) {
                if (sem != NULL) {
                        // printf("w[%d] - ", getPid());
                        sem_wait(sem);
                }
                slowInc(&global, inc);
                if (sem != NULL) {
                        // printf("p[%d]\n", getPid());
                        sem_post(sem);
                }
        }

        // if (use_sem)
        //         sem_close(sem);

        return 0;
}

int test_sync(uint64_t argc, char *argv[])
{ //{n, use_sem, 0}
        uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

        if (argc != 3) {
                return -1;
                printf("Error: Two arguments Needed:\n\t- Number of repetitions\n\t- Semaphore used\n");
        }

        char *argvDec[] = { "Dec Process", argv[1], "-1", argv[2], NULL };
        char *argvInc[] = { "Inc Process", argv[1], "1", argv[2], NULL };

        printf("Main Process n: [%s] - useSem: [%s]\n", argv[1], argv[2]);
        global = 0;

        uint64_t i;
        for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
                /* pids[i] = my_create_process("my_process_inc", 3, argvDec); */
                pids[i] = createProcess("my_process_inc", my_process_inc, 3, argvDec, 0);
                /* pids[i + TOTAL_PAIR_PROCESSES] = */
                /*         my_create_process("my_process_inc", 3, argvInc); */
                pids[i + TOTAL_PAIR_PROCESSES] =
                        createProcess("my_process_inc", my_process_inc, 3, argvInc, 0);
        }

        for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
                waitPID(pids[i]);
                // printf("Done [%d]\n", pids[i]);
                waitPID(pids[i + TOTAL_PAIR_PROCESSES]);
                // printf("Done [%d]\n", pids[i + TOTAL_PAIR_PROCESSES]);
        }

        int sig = 0;
        if (global < 0) {
                sig = 1;
                global *= -1;
        }
        printf("Final value: %s%d\n", sig ? "-" : "", global);

        return 0;
}

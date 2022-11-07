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
/* #include "syscall.h" */
#include <stdlib.h> /* NULL */
#include <processManagement.h> /* createProcess(); */
#include <processes.h> /* commandBlock(); commandKill(); commandNice();
                        * commandUnblock(); */
#include <semaphoreUser.h> /* sem_open(); sem_wait(); sem_post(); 
                            * sem_close(); */

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global; //shared memory

void slowInc(int64_t *p, int64_t inc)
{
        uint64_t aux = *p;
        giveUpCPU(); //This makes the race condition highly probable
        aux += inc;
        *p = aux;
}

int my_process_inc(int argc, char *argv[])
{
        uint64_t n;
        int8_t inc;
        int8_t use_sem;
        sem_t *sem = NULL;

        if (argc != 3)
                return -1;

        if ((n = satoi(argv[0])) <= 0)
                return -1;
        if ((inc = satoi(argv[1])) == 0)
                return -1;
        if ((use_sem = satoi(argv[2])) < 0)
                return -1;

        if (use_sem)
                sem = sem_open(SEM_ID, 1);
        if (sem == NULL) {
                printf("test_sync: ERROR opening semaphore\n");
                return -1;
        }

        uint64_t i;
        for (i = 0; i < n; i++) {
                if (use_sem)
                        sem_wait(sem);
                slowInc(&global, inc);
                if (use_sem)
                        sem_post(sem);
        }

        if (use_sem)
                sem_close(sem);

        return 0;
}

int test_sync(uint64_t argc, char *argv[])
{ //{n, use_sem, 0}
        uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

        if (argc != 2)
                return -1;

        char *argvDec[] = { argv[0], "-1", argv[1], NULL };
        char *argvInc[] = { argv[0], "1", argv[1], NULL };

        global = 0;

        uint64_t i;
        for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
                /* pids[i] = my_create_process("my_process_inc", 3, argvDec); */
                pids[i] = createProcess("my_process_inc", NULL, 3, argvDec, 0);
                /* pids[i + TOTAL_PAIR_PROCESSES] = */
                /*         my_create_process("my_process_inc", 3, argvInc); */
                pids[i + TOTAL_PAIR_PROCESSES] =
                        createProcess("my_process_inc", NULL, 3, argvInc, 0);
        }

        // TODO: What's this?
        for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) { 
                waitPID(pids[i]); 
                waitPID(pids[i + TOTAL_PAIR_PROCESSES]);
        }

        printf("Final value: %d\n", global);

        return 0;
}

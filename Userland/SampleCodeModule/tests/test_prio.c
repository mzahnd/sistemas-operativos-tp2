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

#define MINOR_WAIT \
        1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT \
        10000000 // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 0 //TODO: Change as required
#define MEDIUM 1 //TODO: Change as required
#define HIGHEST 2 //TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = { LOWEST, MEDIUM, HIGHEST };

int test_prio(int argc, char *argv[])
{
        int64_t pids[TOTAL_PROCESSES];
        uint64_t i;

        /* char *argv[] = { 0 }; */
        /* pids[i] = my_create_process("endless_loop_print", 0, argv); */
        for (i = 0; i < TOTAL_PROCESSES; i++) {
                int cargc = 0;
                char **cargv = create_argv("endless_loop_print", &cargc, 1, 0);
                pids[i] = createProcess("endless_loop_print",
                                        &endless_loop_print, cargc, cargv, 0);
        }

        bussy_wait(WAIT);
        printf("\nCHANGING PRIORITIES...\n");

        /* my_nice(pids[i], prio[i]); */
        for (i = 0; i < TOTAL_PROCESSES; i++) {
                int cargc = 0;
                char **cargv =
                        create_argv("commandNice", &cargc, 2, pids[i], prio[i]);
                commandNice(cargc, cargv);
        }

        bussy_wait(WAIT);
        printf("\nBLOCKING...\n");

        /* my_block(pids[i]); */
        for (i = 0; i < TOTAL_PROCESSES; i++) {
                int cargc = 0;
                char **cargv = create_argv("commandBlock", &cargc, 1, pids[i]);
                commandBlock(cargc, cargv);
        }

        printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

        /* my_nice(pids[i], MEDIUM); */
        for (i = 0; i < TOTAL_PROCESSES; i++) {
                int cargc = 0;
                char **cargv =
                        create_argv("commandNice", &cargc, 2, pids[i], MEDIUM);
                commandNice(cargc, cargv);
        }

        printf("UNBLOCKING...\n");

        /* my_unblock(pids[i]); */
        for (i = 0; i < TOTAL_PROCESSES; i++) {
                int cargc = 0;
                char **cargv =
                        create_argv("commandUnblock", &cargc, 1, pids[i]);
                // Unblock
                commandBlock(cargc, cargv);
        }

        bussy_wait(WAIT);
        printf("\nKILLING...\n");

        /* my_kill(pids[i]); */
        for (i = 0; i < TOTAL_PROCESSES; i++) {
                int cargc = 0;
                char **cargv = create_argv("commandKill", &cargc, 1, pids[i]);
                commandKill(cargc, cargv);
        }
}

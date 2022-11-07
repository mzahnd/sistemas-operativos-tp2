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
#include <stdio.h>
#include <tests/test_util.h>
/* #include "syscall.h" */
#include <stdlib.h> /* NULL */
#include <processManagement.h> /* createProcess(); */
#include <processes.h> /* commandBlock(); commandKill(); commandUnblock(); */

enum State { RUNNING, BLOCKED, KILLED };

typedef struct P_rq {
        int32_t pid;
        enum State state;
} p_rq;

int64_t test_processes(uint64_t argc, char *argv[])
{
        uint8_t rq;
        uint8_t alive = 0;
        uint8_t action;
        uint64_t max_processes;
        char *argvAux[] = { 0 };

        if (argc != 1)
                return -1;

        if ((max_processes = satoi(argv[0])) <= 0)
                return -1;

        p_rq p_rqs[max_processes];

        while (1) {
                // Create max_processes processes
                for (rq = 0; rq < max_processes; rq++) {
                        p_rqs[rq].pid = createProcess(
                                "endless_loop", &endless_loop, 0, argvAux, 0);

                        if (p_rqs[rq].pid == -1) {
                                printf("test_processes: ERROR creating process\n");
                                return -1;
                        } else {
                                p_rqs[rq].state = RUNNING;
                                alive++;
                        }
                }

                // Randomly kills, blocks or unblocks processes until every one has been killed
                while (alive > 0) {
                        int cargc = 0;
                        char **cargv = NULL;

                        for (rq = 0; rq < max_processes; rq++) {
                                action = GetUniform(100) % 2;

                                switch (action) {
                                case 0:
                                        if (p_rqs[rq].state == RUNNING ||
                                            p_rqs[rq].state == BLOCKED) {
                                                cargv = create_argv(
                                                        "test_processes_kill",
                                                        &cargc, 1,
                                                        p_rqs[rq].pid);

                                                if (commandKill(cargc, cargv) ==
                                                    -1) {
                                                        printf("test_processes: ERROR killing process\n");
                                                        return -1;
                                                }
                                                p_rqs[rq].state = KILLED;
                                                alive--;
                                        }
                                        break;

                                case 1:
                                        if (p_rqs[rq].state == RUNNING) {
                                                cargv = create_argv(
                                                        "test_processes_block",
                                                        &cargc, 1,
                                                        p_rqs[rq].pid);

                                                if (commandBlock(cargc,
                                                                 cargv) == -1) {
                                                        printf("test_processes: ERROR blocking process\n");
                                                        return -1;
                                                }
                                                p_rqs[rq].state = BLOCKED;
                                        }
                                        break;
                                }
                        }

                        // Randomly unblocks processes
                        for (rq = 0; rq < max_processes; rq++)
                                if (p_rqs[rq].state == BLOCKED &&
                                    GetUniform(100) % 2) {
                                        cargv = create_argv(
                                                "test_processes_unblock",
                                                &cargc, 1, p_rqs[rq].pid);

                                        // Unblock
                                        if (commandBlock(cargc, cargv) == -1) {
                                                printf("test_processes: ERROR unblocking process\n");
                                                return -1;
                                        }
                                        p_rqs[rq].state = RUNNING;
                                }
                }
        }
}

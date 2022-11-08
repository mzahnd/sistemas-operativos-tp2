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
#include <processes.h>
#include <syscalls_definitions.h>
#include <syscalls_asm.h>
#include <stdio.h>
#include <stdlib.h>

int schedulerInfo(int argc, char **argv)
{
        schInfo_t info;
        getSchedulerInfoSyscall(&info);
        printf("\n");
        printf("NAME\tPID\tPPID\tSTATUS\tFOREGROUND\tSTACK\n");
        for (int i = 0; i < info.totalProcesses; i++) {
                char *status;
                if (info.processes[i].status == 1) {
                        status = "READY";
                } else if (info.processes[i].status == 2) {
                        status = "BLOCKED";
                } else if (info.processes[i].status == 3) {
                        status = "KILLED";
                } else {
                        status = "ERROR";
                }
                printf("%s\t%d\t%d\t%s\t%d\t%X\n", info.processes[i].name, //-V576
                       info.processes[i].pid, info.processes[i].ppid, status, info.processes[i].foreground, info.processes[i].stack);
        }

        return 0;
}

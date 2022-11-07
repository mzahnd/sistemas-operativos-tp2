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
#include <syscalls_asm.h>
#include <stdio.h>
#include <stdlib.h>

// static int block(int pid) {
//     int ans;
//     blockSyscall(pid, &ans);
//     return ans;
// }

int commandBlock(int argc, char **argv)
{
        unsigned int id = atoi(argv[1]);

        changeProcessStatusSyscall(id);
        // if(block(id) == 0) {
        //     printf("Process successfully blocked\n");
        // } else {
        //     printf("Process failed to block\n");
        // }

        return 0;
}
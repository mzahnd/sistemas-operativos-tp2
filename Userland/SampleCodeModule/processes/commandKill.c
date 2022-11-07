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

// static int kill(int pid){
//     int ans;
//     killProcessSyscall(pid);
//     return ans;
// }

int commandKill(int argc, char **argv) {
    printf("\n");
    unsigned int id = atoi(argv[1]);
    // if(kill(id) == 0) 
    //     printf("Process successfully killed\n");
    // else
    //     printf("Process kill failed\n");
    killProcessSyscall(id);
    return 0;
}


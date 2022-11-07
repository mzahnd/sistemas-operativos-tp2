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
#include <processManagement.h>
#include <syscalls_asm.h>
#include <stdio.h>
#include <stdlib.h>


int commandLoop(int argc, char **argv) {

    if (argc != 2) {
        printf("ERROR: Must have a interval parameter in seconds!\n");
        return -1;
    }

    int interval = atoi(argv[1]);
    printf("Interval: %s\n", argv[1]);
    
    while (1) {
        printf("Hello, this is the LOOP command. Current PID is %d\n", getPid());
        sleep(3);
    }

    return 0;
}
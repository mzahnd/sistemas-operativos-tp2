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

#define EOF -1


int commandWc(int argc, char **argv){
    char c;
    int lineCount = 0;

    while ((c = getChar()) != EOF) {
        printf(c);

        if (c == '\n') {
            lineCount++;
        }
    }
    printf("\n\nLine Count: %d\n", lineCount);
    processKiller();
    return 1;
}
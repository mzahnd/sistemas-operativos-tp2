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

#define EOF 0
#define MAX_BUFFER_SIZE 2048

int commandWc(int argc, char **argv)
{
        int lineCount = 0;

        char c;
        while ((c = getChar()) != EOF) {
                if (c == '\n') {
                        lineCount++;
                }
        }
        printf("Line Count: %d\n", lineCount);
        return 0;
}
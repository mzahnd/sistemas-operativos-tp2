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

int commandWc(int argc, char **argv)
{
        char c;
        int lineCount = 0;

        char buffer[1024] = { 0 };
        read(STDIN, buffer, 1024);
        for (int i = 0; i < 1024 && buffer[i] != 0; i++) {
                if (buffer[i] == '\n') {
                        lineCount++;
                }
        }

        // while ((c = getChar()) != EOF) {
        //         putChar(c);

        //         if (c == '\n') {
        //                 lineCount++;
        //         }
        // }
        printf("[%s]\n", buffer);
        printf("\n\nLine Count: %d\n", lineCount);
        processKiller();
        return 0;
}
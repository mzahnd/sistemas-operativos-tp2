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

int commandFilter(int argc, char **argv)
{
        char c;

        char buffer[128];
        unsigned int i = 0;

        while ((c = getChar()) != 0) {
                if (!isVowel(c)) {
                        // strcat(buffer, c, &i);
                        buffer[i++] = c;
                }
        }
        buffer[i] = 0;
        printf("\n");
        printf("Filtered word: %s\n", buffer);
        processKiller();

        return 0;
}
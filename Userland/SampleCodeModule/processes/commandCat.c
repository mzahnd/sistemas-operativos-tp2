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

#define CAT_BREAKING '\0'

int commandCat(int argc, char **argv)
{
        char c;
        while ((c = getChar()) != CAT_BREAKING) {
                printf("%c", c);
        }
        printf("\n");
        processKiller();
        return 0;
}
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
/* sampleCodeModule.c */

#include <syscalls_asm.h>
#include <colors.h>
#include <shells.h>
#include <stdio.h>
#include <stdint.h>
#include <BetterShell/betterShell.h>

/* char *v = (char *)0xB8000 + 79 * 2; */

int main()
{
        runShell(0, NULL);
        //uint64_t errCode = getError();

        //init_shell(errCode);
        // init_shell();
        // clearScreen(0xFFFFFF);
        // drawString(0, 0, "Hola", 4, 0xFFFFFF, 0x000000, 1, 0);
        // drawRect(2, 2, 5, 5, 0xFFFFFF);
        return 0;
}

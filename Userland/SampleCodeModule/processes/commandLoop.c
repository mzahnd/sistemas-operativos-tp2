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

int commandLoop(int argc, char **argv)
{
        while (1) { // -V776
                printf("Hello, this is the LOOP command." // -V576
                       " Current PID is %d\n",
                       getPid());
                sleep(1);
        }

        return 0;
}

int commandActiveLoop(int argc, char **argv)
{
        while (1) { // -V776
                for (int i = 0; i < 500000000; i++)
                        ; // -V529

                printf("Hello, this is the ACTIVE LOOP command." // -V576
                       " Current PID is %d\n",
                       getPid());
        }
}

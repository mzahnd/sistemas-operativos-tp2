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
#ifndef TEST_PROCESS
#define TEST_PROCESS

#include <processes.h>
#include <stdio.h>

int testProcess(int argc, char **argv)
{
        for (int i = 0; i < 10; i++) {
                printf("HOLA %d\n", i);
        }
        return 0;
}

#endif /* TEST_PROCESS */

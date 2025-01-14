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
/* _loader.c */
#include <stdint.h>
#include <stringUtils.h> /* memset(); */

extern char bss;
extern char endOfBinary;

int main();

int _start()
{
        //Clean BSS
        memset(&bss, 0, &endOfBinary - &bss);

        return main();
}

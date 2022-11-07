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
#ifndef PROCESS_MANAGEMENT_H
#define PROCESS_MANAGEMENT_H
#include <stdint.h>

uint64_t createProcess(char *name, int (*mainF)(int, char **), int argc,
                       char **argv, unsigned int foreground);
uint64_t createProcessWithFD(char *name, int (*mainF)(int, char **), int argc,
                             char **argv, unsigned int foreground,
                             unsigned int stdin, unsigned int stdout);
void waitPID(uint64_t pid);
void sleep(unsigned int seconds);
unsigned int getPid();

#endif /* PROCESS_MANAGEMENT_H */

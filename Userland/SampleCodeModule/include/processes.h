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
#ifndef PROCESSES_H
#define PROCESSES_H

int testProcess(int, char **);
int schedulerInfo(int, char **);
int testPipes(int, char **);
int testWrite(int argc, char **argv);
int testRead(int argc, char **argv);

#endif /* PROCESSES_H */

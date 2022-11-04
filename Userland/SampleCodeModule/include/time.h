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
#ifndef TIME_H
#define TIME_H

#include <stdint.h>

int getSeconds();
int getMinutes();
int getHours();
int getDays();
int getMonth();
int getYear();
int getTicks();
void setTimerFunction(int index, int tickInterval, void (*)());
void deleteTimerFunction(int index);

#endif /* TIME_H */

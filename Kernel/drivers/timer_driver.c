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
#ifndef TIME_DRIVER
#define TIME_DRIVER

#include <stdint.h>
#include <timer_driver.h>

#define TOTAL_METHODS 16

static void (*ticksMethods[TOTAL_METHODS])(void);
static int intervalTicks[TOTAL_METHODS];

static uint64_t ticks = 0;

void timeHandler()
{
        ticks++;
        for (int i = 0; i < TOTAL_METHODS; i++) {
                if (intervalTicks[i] > 0 && ticks % intervalTicks[i] == 0) {
                        (*ticksMethods[i])();
                }
        }
}

uint64_t getTicks()
{
        return ticks;
}

void initializeTickMethods()
{
        for (int i = 0; i < TOTAL_METHODS; i++) {
                ticksMethods[i] = 0;
                intervalTicks[i] = 0;
        }
}

int secondsElapsed()
{
        return ticks / 18;
}

void setTickMethod(int methodIndex, int tickInterval, void (*function)())
{
        if (methodIndex < TOTAL_METHODS) {
                intervalTicks[methodIndex] = tickInterval;
                ticksMethods[methodIndex] = function;
        }
}

void deleteTickMethod(int methodIndex)
{
        if (methodIndex < TOTAL_METHODS) {
                intervalTicks[methodIndex] = 0;
                ticksMethods[methodIndex] = 0;
        }
}

#endif /* TIME_DRIVER */

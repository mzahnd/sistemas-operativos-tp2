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
#include <syscalls_asm.h>
#include <stdint.h>
#include <time.h>

int getSeconds()
{
        uint8_t target;
        getTimeSyscall(0, &target);
        return target;
}

int getMinutes()
{
        uint8_t target;
        getTimeSyscall(1, &target);
        return target;
}

int getHours()
{
        uint8_t target;
        getTimeSyscall(2, &target);
        return target;
}

int getDays()
{
        uint8_t target;
        getTimeSyscall(3, &target);
        return target;
}

int getMonth()
{
        uint8_t target;
        getTimeSyscall(4, &target);
        return target;
}

int getYear()
{
        uint8_t target;
        getTimeSyscall(5, &target);
        return target;
}

int getTicks()
{
        uint64_t ticks;
        // getTicksSyscall(&ticks);
        getTicksSyscall(&ticks);
        return ticks;
}

void setTimerFunction(int index, int tickInterval, void (*func)())
{
        setTimerFunctionSyscall(index, tickInterval, func);
}

void deleteTimerFunction(int index)
{
        deleteTimerFunctionSyscall(index);
}

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
#ifndef DATE_DRIVER
#define DATE_DRIVER

// #include <date_asm.h>
#include <date_driver.h>
#include <stdint.h>
#include <lib.h>

uint8_t getSeconds()
{
        return RTC(0);
}

uint8_t getMinutes()
{
        return RTC(2);
}

uint8_t getHours()
{
        return RTC(4);
}

uint8_t getDay()
{
        return RTC(7);
}

uint8_t getMonth()
{
        return RTC(8);
}

uint8_t getYear()
{
        return RTC(9);
}

#endif /* DATE_DRIVER */

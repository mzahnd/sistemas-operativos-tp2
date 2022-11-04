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
#ifndef DATE_DRIVER_H
#define DATE_DRIVER_H

#include <stdint.h>

uint8_t getSeconds();
uint8_t getMinutes();
uint8_t getHours();
uint8_t getDay();
uint8_t getMonth();
uint8_t getYear();

#endif /* DATE_DRIVER_H */

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
#ifndef IO_DRIVER_H
#define IO_DRIVER_H

#include <stdint.h>
#include <regi.h>

void saveRegisters(registerStruct *registers);
void getRegisters(uint64_t *arr);
void getBytesFromAddress(uint64_t address, uint64_t *target,
                         uint8_t totalBytes);
void saveErrCode(uint64_t err);
uint64_t getErrCode();

#endif /* IO_DRIVER_H */

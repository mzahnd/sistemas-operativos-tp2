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
#ifndef EXCEPTIONS
#define EXCEPTIONS

#include <regi.h>
#include <stdint.h>
#include <IO_driver.h>
#include <exceptions.h>
#include <video_driver.h>

void exc_0h(registerStruct *registers)
{
        saveErrCode(0);
        saveRegisters(registers);
}

void exc_6h(registerStruct *registers)
{
        saveErrCode(6);
        saveRegisters(registers);
}

void readError(uint64_t *target)
{
        uint64_t error = getErrCode();
        *target = error;
}

#endif /* EXCEPTIONS */

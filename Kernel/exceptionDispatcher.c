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
#ifndef EXCEPTION_DISPATCHER
#define EXCEPTION_DISPATCHER

#include <regi.h>
#include <interrupts.h>
#include <video_driver.h>
#include <exceptions.h>

void exceptionDispatcher(uint64_t exc, registerStruct *registers)
{
        switch (exc) {
        case 0:
                exc_0h(registers);
                break;
        case 6:
                exc_6h(registers);
                break;
        }
}

#endif /* EXCEPTION_DISPATCHER */

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
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

void _cli();
void _sti();
void _hlt();
void picSlaveMask(uint8_t mask);
void picMasterMask(uint8_t mask);
void _irq00Handler(void);
void _irq01Handler(void);
void _irq80Handler(void);
void _exception00Handler(void);
void _exception06Handler(void);
void saveInitialConditions(void *address);
void _sendEOI(void);
void forceTimerTick(void);

#endif /* INTERRUPTS_H */

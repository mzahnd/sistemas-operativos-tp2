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
#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#define CTRL_L 29
#define SHIFT_L 42
#define SHIFT_R 54

#define BLOCK_M 58

#define KEY_RELEASED 128
#define KEYS 59

#define F1 59
#define F12 88

#define KEY_UP 72
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_DOWN 80

#include <regi.h>
#include <stdint.h>

void keyboardHandler(registerStruct *);
void readKeyboard(int *buff, uint64_t size, uint64_t *count);
void bufferEmpty(uint64_t *target);
void isMayus(uint64_t *target);
void initializeFunctionKeys();
void setFunctionKeyMethod(uint64_t index, void (*function)());

#endif /* KEYBOARD_DRIVER_H */

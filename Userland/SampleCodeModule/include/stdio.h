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
#ifndef STDIO_H
#define STDIO_H

#include <stdint.h>
#include <sys/types.h>

#define KEY_UP (char)200
#define KEY_LEFT (char)203
#define KEY_RIGHT (char)205
#define KEY_DOWN (char)208

#define STDIN 0
#define STDOUT 1

void scanf(char *buffer);
void printf(char *fmt, ...);
void setConsoleUpdateFunction(void (*f)(char *, int));
void putChar(char ch);
void setFunctionKey(int index, void (*func)());
int getChar();
uint64_t getError();
void format(char *str, int value);

ssize_t read(int fd, char *buf, size_t count);
ssize_t write(int fd, const char *buf, size_t count);

#endif /* STDIO_H */

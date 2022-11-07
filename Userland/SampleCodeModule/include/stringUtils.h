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
#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stddef.h> /* size_t */

unsigned int strlen(char *);
size_t strnlen(const char *s, size_t maxlen);
char *strcat(char *destination, const char *source);
char *strcpy(char *destination, const char *source);
char *strncpy(char *destination, const char *source, unsigned int num);
int strcmp(const char *, const char *);
void reverseStr(char str[]);
int isAlpha(int ch);
int isDigit(int ch);
int isSymbol(int ch);
int isWritable(int ch);
void *memset(void *dest, int c, size_t len);
void *memcpy(void *dest, const void *src, size_t len);

#endif /* STRING_UTILS_H */

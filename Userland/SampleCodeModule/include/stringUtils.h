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

unsigned int strlen(char *);
char *strcat(char *destination, const char *source);
char *strcpy(char *destination, const char *source);
char* strncpy(char* destination, const char* source, unsigned int num);
int strcmp(const char *, const char *);
void reverseStr(char str[]);
int isAlpha(int ch);
int isDigit(int ch);
int isSymbol(int ch);
int isWritable(int ch);

#endif /* STRING_UTILS_H */

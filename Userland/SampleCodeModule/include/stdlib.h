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
#ifndef STDLIB_H
#define STDLIB_H

#include <stddef.h> /* size_t */
#include <syscalls_definitions.h> /* mem_info_t */

#ifndef NULL
#define NULL 0
#endif /* NULL */

#ifndef EOF
#define EOF -1
#endif /* EOF */

int atoi(char *str);
int intToString(unsigned long long num, char *buffer);
int intToBase(unsigned long long num, int base, char *buffer);
int iabs(int num);
int atohex(char *str);
char toUpper(char letter);
char toLower(char letter);
int isVowel(char letter);
int round(double number);
void reverseStr(char str[]);
void *malloc(unsigned int size);
void *calloc(size_t nmemb, size_t size);
void free(void *ptr);
void giveUpCPU();
mem_info_t *mem_getinformation();

int rand();
void srand(unsigned seed);

#endif /* STDLIB_H */

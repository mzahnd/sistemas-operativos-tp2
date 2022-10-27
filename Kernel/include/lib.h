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
#ifndef LIB_H
#define LIB_H

#include <stddef.h> /* size_t */
#include <stdint.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

// void *sosbrk(uint64_t size);
void *somemset(void *dest, int32_t c, uint64_t n);
void *somemcpy(void *dest, const void *src, uint64_t n);

/**
 * djb2 string hashing algorithm
 *
 * Source: http://www.cse.yorku.ca/~oz/hash.html
 */
uint64_t djb2 (const unsigned char* str);

size_t strnlen(const char *s, size_t maxlen);

char *cpuVendor(char *result);

uint8_t RTC(uint8_t mode);

#endif /* LIB_H */

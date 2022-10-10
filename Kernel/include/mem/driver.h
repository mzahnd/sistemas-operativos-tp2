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
#ifndef DRIVER_H
#define DRIVER_H

#include <stddef.h> /* size_t */
#include <stdint.h>

/* Type to use for aligned memory operations.
 *
 * Should normally be the biggest type supported by a single load and store.
 * Must be an unsigned type.
 *
 * Motivation: memset() glibc 2.36
 * https://sourceware.org/git/?p=glibc.git;a=blob;f=string/memcmp.c;h=40029474e6c9206e5c38711d60b011768f422f44;hb=refs/heads/release/2.36/master
 */
#define op_t unsigned long int
#define OPSIZ (sizeof(op_t))

// void *sosbrk(uint64_t size);
void *somemset(void *dest, int c, size_t n);
void *somemcpy(void *dest, void *src, size_t n);

#endif /* DRIVER_H */

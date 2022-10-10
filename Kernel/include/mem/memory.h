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
#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h> /* size_t */
#include <stdint.h>

// Pure64 free memory starts at 0x100000. Leave 256 MiB for non-heap stuff
#define MEM_HEAP_START_ADDR (0x100000 << 8)
#define MEM_HEAP_SIZE (256 * 1024 * 1024) // 256 MiB

typedef union MEMORY_BLOCK memory_block;

union MEMORY_BLOCK {
        struct {
                memory_block *ptr;
                uint32_t size;
        };

        uint64_t __to_align; /* Do not use */
};

void *somalloc(size_t size);

void *socalloc(size_t nmemb, size_t size);

void *sorealloc(void *ptr, size_t size);

void sofree(void *ptr);

#endif /* MEMORY_H */

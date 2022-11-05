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

// HEAP_SIZE can be externally defined
#ifndef MEM_HEAP_SIZE
#define MEM_HEAP_SIZE (256 * 1024 * 1024) // 256 MiB
#endif

typedef struct {
        unsigned int n_reserved_blocks;

        size_t reserved_size;
        size_t user_size;
        size_t free_size;

        // Allocated memory blocks are between
        // first_address and MEM_HEAP_START_ADDR + MEM_HEAP_SIZE
        void *first_address;
} somem_info_t;

void *somalloc(size_t size);

void *socalloc(size_t nmemb, size_t size);

void sofree(void *ptr);

somem_info_t *somem_getinformation();

#endif /* MEMORY_H */

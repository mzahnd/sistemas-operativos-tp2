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

#include <mem/driver.h>
#include <mem/memory.h>

#define MEM_HEAP_FINAL_ADDR (MEM_HEAP_START_ADDR + MEM_HEAP_SIZE)

/* Prototypes */
static void mem_init();
// static memory_block *request_more_space(uint32_t nu);

/* Globals */
static memory_block *heap_freep = NULL;

void *somalloc(size_t size)
{
        if (size == 0)
                return NULL;

        uint32_t nunits =
                (size + sizeof(memory_block) - 1) / sizeof(memory_block) + 1;

        // Heap has not been initialized
        if (heap_freep == NULL)
                mem_init();

        for (memory_block *prevp = heap_freep, *p = heap_freep->ptr; /* */;
             prevp = p, p = p->ptr) {
                if (p->size >= nunits) {
                        if (p->size == nunits) { /* Exact size */
                                prevp->ptr = p->ptr;
                        } else { /* Allocate tail end */
                                p->size -= nunits;
                                p += p->size;
                                p->size = nunits;
                        }

                        heap_freep = prevp;
                        return (void *)(p + 1);
                }

                if (p == heap_freep) {
                        /* p = request_more_space(nunits); // Not implemented
                         if (p == NULL) */
                        return NULL;
                }
        }
}

void *socalloc(size_t nmemb, size_t size)
{
        if (nmemb == 0 || size == 0)
                return NULL;

        void *ptr = somalloc(size * nmemb);
        if (ptr == NULL)
                return NULL;

        somemset(ptr, 0, size * nmemb);

        return ptr;
}

void *sorealloc(void *ptr, size_t size)
{
        if (ptr == NULL && size == 0)
                return NULL;
        else if (ptr == NULL)
                return somalloc(size);
        else if (size == 0) {
                sofree(ptr);
                return NULL;
        }

        memory_block *block = (memory_block *)ptr - 1;

        if (size > block->size) {
                // New block needed
                void *new_ptr = somalloc(size);
                if (new_ptr == NULL)
                        return NULL;

                somemcpy(new_ptr, ptr, block->size);

                return new_ptr;
        } else if ((size < block->size && block->size >= 1024 * 1024) ||
                   (size < block->size - block->size / 2)) {
                // New size will left at least 1 MiB unusuable
                // or is at least 1/2 of the original block size.
                // This "justifies" shrinking the block.

                memory_block *new_block = ptr + size;
                new_block->size = block->size - size;
                // sofree() shrinks
                sofree(new_block);

                block->size = size;
        }

        // Use the same block
        return ptr;
}

void sofree(void *ptr)
{
        if (ptr < (void *)MEM_HEAP_START_ADDR ||
            ptr > (void *)MEM_HEAP_FINAL_ADDR)
                return;

        memory_block *block = (memory_block *)ptr - 1;
        memory_block *p = NULL;

        // Avoid memory overrun
        if ((void *)(block + block->size) > (void *)MEM_HEAP_FINAL_ADDR)
                block->size = MEM_HEAP_FINAL_ADDR - (size_t)block;

        for (p = heap_freep; !(block > p && block < p->ptr); p = p->ptr) {
                if (p >= p->ptr && (block > p || block < p->ptr))
                        // Freed block is at start or end of arena
                        break;
        }

        if (block + block->size == p->ptr) {
                // Join upper free block
                block->size += p->ptr->size;
                block->ptr = p->ptr->ptr;

                // Avoid possible memory overrun
                if ((void *)(block + block->size) > (void *)MEM_HEAP_FINAL_ADDR)
                        block->size = MEM_HEAP_FINAL_ADDR - (size_t)block;
        } else {
                block->ptr = p->ptr;
        }

        if (p + p->size == block) {
                // Join lower free block
                p->size += block->size;
                p->ptr = block->ptr;

                // Avoid possible memory overrun
                if ((void *)(p + p->size) > (void *)MEM_HEAP_FINAL_ADDR)
                        p->size = MEM_HEAP_FINAL_ADDR - (size_t)p;
        } else {
                p->ptr = block;
        }

        heap_freep = p;
}

static void mem_init()
{
        heap_freep = (memory_block *)MEM_HEAP_START_ADDR;
        heap_freep->size = (MEM_HEAP_SIZE + sizeof(memory_block) - 1) /
                                   sizeof(memory_block) +
                           1;
        heap_freep->ptr = heap_freep;
}

/*
 * TODO: implement sosbrk()
static memory_block *request_more_space(uint32_t nu)
{
        char *cp;
        memory_block *up;

        if (nu < NALLOC) {
                nu = NALLOC;
        }

        cp = sosbrk(nu * sizeof(memory_block));
        if (cp == NULL) {
                return NULL;
        }
        up = (memory_block *)cp;
        up->size = nu;
        free((void *)(up + 1));
        return heap_freep;
}
*/

// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**
 * This file is part of sistemas-operativos-tp2
 * Licensed under BSD 3-Clause "New" or "Revised" License.
 * Copyright (c) 2022 Flores Levalle, M.
 *                    López, P.
 *                    Sierra Pérez, C.
 *                    Zahnd, M. E.
 *
 * This source code has been insipred in FreeRTOS heap_4 and K&R malloc.
 *
 * See: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/portable/MemMang/heap_4.c
 */
#include <lib.h>
#include <mem/memory.h>

/* ------------------------------ */

typedef struct MEMORY_BLOCK {
        struct MEMORY_BLOCK *next;
        size_t size; /* Total block size */
        size_t user_size; /* User available */
} memory_block;

/* ------------------------------ */

#ifdef TESTING
#pragma message("TESTING defined")
#undef MEM_HEAP_START_ADDR
#define MEM_HEAP_START_ADDR (test_get_mem_heap_start_addr())
#endif /* TESTING */

/* ------------------------------ */

/* Masks and alignment */
#define BYTE_ALIGNMENT 32
#define BYTE_ALIGNMENT_MASK (0x001f)

/* Arithmetic functions */
#define ADD_WILL_OVERFLOW(a, b) ((a) > (MEM_HEAP_SIZE - (b)))
/* Move a pointer by size to the right and return as pointer to void */
#define ADD_VOID_PTR_SIZE(ptr, size) \
        ((void *)(((void *)(ptr)) + ((size_t)(size))))
/* Move a pointer by size to the left and return as pointer to void */
#define SUB_VOID_PTR_SIZE(ptr, size) \
        ((void *)(((void *)(ptr)) - ((size_t)(size))))

/* Block related macros and definitions */
#define MEMBLOCK_SIZE_DEF                                        \
        ((sizeof(memory_block) + (size_t)(BYTE_ALIGNMENT - 1)) & \
         ~((size_t)BYTE_ALIGNMENT_MASK))

#define MINIMUM_BLOCK_SIZE ((size_t)(memblock_size << 1))

/* Get block header from user pointer (created with somalloc) */
#define ALLOC_BLOCK_HEADER(blk) \
        ((memory_block *)SUB_VOID_PTR_SIZE(blk, memblock_size))

/* ------------------------------ */

/* Prototypes */
static void mem_init();
inline static memory_block *move_to_free_header_to_real_start(void *ptr);
// static memory_block *request_more_space(uint32_t nu); // Not implemented

#ifdef TESTING
inline static uint64_t test_get_mem_heap_start_addr();
#endif /* TESTING */

/* ------------------------------ */

/* Globals */
// Correctly align the structure at the begginig of each allocated memory block
static const size_t memblock_size = MEMBLOCK_SIZE_DEF;

// List with the list
static memory_block *heap_freep = NULL;

/* ------------------------------ */

void *somalloc(size_t wanted_size)
{
        memory_block *to_return = NULL;
        size_t extra_space_required = 0;

        // Heap has not been initialized
        if (heap_freep == NULL)
                mem_init();

        if (wanted_size > 0)
                extra_space_required = memblock_size + BYTE_ALIGNMENT -
                                       (wanted_size & BYTE_ALIGNMENT_MASK);

        while (wanted_size + extra_space_required < MINIMUM_BLOCK_SIZE) {
                wanted_size++;

                // Calculate again
                if (wanted_size > 0)
                        extra_space_required =
                                memblock_size + BYTE_ALIGNMENT -
                                (wanted_size & BYTE_ALIGNMENT_MASK);
        }

        if (ADD_WILL_OVERFLOW(wanted_size, extra_space_required) == 0 &&
            wanted_size + extra_space_required >= MINIMUM_BLOCK_SIZE) {
                int found_valid_block = 0;

                wanted_size += extra_space_required;

                /* Traverse the list */
                memory_block *initial = heap_freep;
                memory_block *prevp = initial;
                memory_block *currp = prevp->next;

                found_valid_block = 1;
                while (currp->size < wanted_size && found_valid_block != 0) {
                        // Again at starting point -> no block with desired size
                        if (currp == initial) {
                                found_valid_block = 0;
                        } else {
                                prevp = currp;
                                currp = currp->next;
                        }
                }

                if (found_valid_block != 0) {
                        to_return = ADD_VOID_PTR_SIZE(
                                currp, currp->size + extra_space_required -
                                               wanted_size);

                        currp->size -= wanted_size;

                        // This block _is_ already inside to_return extra space
                        if (currp->size == 0 && currp == prevp) {
                                currp->next = NULL;
                                currp = NULL;
                                prevp = NULL;
                        } else if (currp->size == 0) {
                                // Remove block from list
                                prevp->next = currp->next;
                                currp = NULL;
                        }

                        // The block is being returned, it has no "next" block
                        ALLOC_BLOCK_HEADER(to_return)->next = NULL;
                        ALLOC_BLOCK_HEADER(to_return)->size = wanted_size;
                        ALLOC_BLOCK_HEADER(to_return)->user_size =
                                wanted_size - extra_space_required;

                        heap_freep = prevp;
                }
        }

        return (void *)to_return;
}

/* ------------------------------ */

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

/* ------------------------------ */

void sofree(void *ptr)
{
        // Prevent user trying to free memory out of heap boundaries
        if (ptr < (void *)MEM_HEAP_START_ADDR || //-V566
            ptr > (void *)(MEM_HEAP_START_ADDR + MEM_HEAP_SIZE)) //-V566
                return;

        memory_block *block_to_free = move_to_free_header_to_real_start(ptr);
        memory_block *prevp = NULL;
        memory_block *nextp = NULL;

        if (heap_freep == NULL) {
                /* First block being freed */

                size_t block_to_free_size = block_to_free->size;

                // Make sure we get all the space reserved for it
                // User space + block header space (alignment included)
                prevp = block_to_free;
                prevp->next = prevp;
                prevp->size = block_to_free_size;
                prevp->user_size = 0;
        } else {
                /* At least one block left */

                prevp = heap_freep;
                nextp = prevp->next;

                // At least two free blocks left
                if (heap_freep != heap_freep->next) {
                        int found_block = 0;
                        do {
                                if (prevp < block_to_free &&
                                    block_to_free < prevp->next) {
                                        // Block is in the middle of the arena
                                        found_block = 1;
                                } else if (prevp > prevp->next &&
                                           prevp->next > block_to_free) {
                                        // Block at the beggining of the arena
                                        found_block = 1;
                                } else if (prevp->next < prevp &&
                                           prevp < block_to_free) {
                                        // Block at the end of the arena
                                        found_block = 1;
                                } else {
                                        prevp = prevp->next;
                                }
                        } while (found_block == 0);

                        nextp = prevp->next;
                }

                // Join left free block
                //
                // | ... | prevp | block_to_free | ... |
                //              OR
                // | ... | nextp | block_to_free | ... |
                //              OR
                // | ... | prevp | ... | block_to_free | [...] | nextp | ... |
                //
                if (ADD_VOID_PTR_SIZE(prevp, prevp->size) ==
                    (void *)block_to_free) {
                        prevp->size += block_to_free->size;

                        block_to_free = prevp;
                } else if (ADD_VOID_PTR_SIZE(nextp, nextp->size) ==
                           (void *)block_to_free) {
                        nextp->size += block_to_free->size;

                        block_to_free = nextp;
                } else if (prevp < block_to_free) {
                        block_to_free->next = nextp;
                        prevp->next = block_to_free;
                }

                // Join right free block
                //
                // | ... | block_to_free | prevp | ... |
                //              OR
                // | ... | block_to_free | nextp | ... |
                //              OR
                // | ... | nextp | [...] | block_to_free | ... | prevp | ... |
                //
                if (ADD_VOID_PTR_SIZE(block_to_free, block_to_free->size) ==
                    (void *)prevp) {
                        if (prevp == nextp) {
                                block_to_free->next = block_to_free;
                        } else {
                                block_to_free->next = nextp;
                        }

                        block_to_free->size += prevp->size;

                        prevp = block_to_free;
                        prevp->user_size = 0;
                } else if (ADD_VOID_PTR_SIZE(block_to_free,
                                             block_to_free->size) ==
                           (void *)nextp) {
                        block_to_free->next = nextp->next;
                        block_to_free->size += nextp->size;

                        nextp = block_to_free;
                } else if (prevp > block_to_free) {
                        block_to_free->next = prevp;
                        nextp->next = block_to_free;
                }
        }

        heap_freep = prevp;
}

/* ------------------------------ */

static void mem_init()
{
        heap_freep = (memory_block *)MEM_HEAP_START_ADDR;
        heap_freep->size = MEM_HEAP_SIZE;
        heap_freep->user_size = 0;
        heap_freep->next = heap_freep;
}
/* ------------------------------ */

inline static memory_block *move_to_free_header_to_real_start(void *ptr)
{
        memory_block tmp = { 0 };
        memory_block *header = ALLOC_BLOCK_HEADER(ptr);
        memory_block *real_start_address = (memory_block *)SUB_VOID_PTR_SIZE(
                ptr, ALLOC_BLOCK_HEADER(ptr)->size -
                             ALLOC_BLOCK_HEADER(ptr)->user_size);

        somemcpy(&tmp, header, sizeof(memory_block));
        somemcpy(real_start_address, &tmp, sizeof(memory_block));

        return real_start_address;
}

/* ------------------------------ */

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

/* ------------------------------ */

#ifdef TESTING
inline static uint64_t test_get_mem_heap_start_addr()
{
        static uint64_t heap_mem_addr[MEM_HEAP_SIZE] = { 0 };

        return (uint64_t)&heap_mem_addr;
}
#endif /* TESTING */

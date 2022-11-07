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
#include <stdio.h> /* printf() */
#include <stdlib.h> /* NULL, mem_getinformation() */

int commandMem(int argc, char **argv)
{
        mem_info_t *info = mem_getinformation();
        printf("********** Memory **********\n");
        if (info == NULL) {
                printf("No information available.\n");
        } else {
                printf("Reserved memory blocks: %d\n", // -V576
                       info->n_reserved_blocks);

                putChar('\n');

                printf("Total reserved size (bytes): %d\n", // -V576
                       info->reserved_size);
                printf("User reserved size (bytes): %d\n", // -V576
                       info->user_size);
                printf("Free size (bytes): %d\n", // -V576
                       info->free_size);

                putChar('\n');

                printf("Start address: 0x%X\n", MEM_HEAP_START_ADDR); // -V576
                printf("End address: 0x%X\n", // -V576
                       (void *)(MEM_HEAP_START_ADDR + MEM_HEAP_SIZE)); // -V566
                printf("Lowest address with a reserved block: 0x%X\n", // -V576
                       info->first_address);

                putChar('\n');

                printf("Heap size (bytes): %d\n", MEM_HEAP_SIZE); // -V576
        }
        printf("****************************\n");

        return 0;
}

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
 * Test provistos por la cátedra.
 */
#include <stdio.h>
#include <stdlib.h>
#include <tests/test_util.h>
#include <processes.h>
/* #include <tests/syscall.h> */
#include <stringUtils.h> /* memset(); */

#define MAX_BLOCKS 128
#define TOTAL_MEM (32 * 1024 * 1024) // Half

typedef struct MM_rq {
        void *address;
        uint32_t size;
} mm_rq;

int test_mm(int argc, char *argv[])
{
        printf("TEST_MM\n");
        mm_rq mm_rqs[MAX_BLOCKS];
        uint8_t rq;
        uint32_t total;
        uint64_t max_memory;

        if (argc != 2) {
                printf("Missing parameter: percentage of memory\n");
                printf("Example: test_mm 50\n");
                return -1;
        }

        if ((max_memory = satoi(argv[1])) <= 0) {
                printf("Wrong parameter: percentage of memory\n");
                printf("Example: test_mm 50\n");
                printf("Got: '%s'\n", argv[1]);
                return -1;
        }

        max_memory *= 0.01 * TOTAL_MEM;

        printf("Testing memory manager with maximum size of %d%%\n",
               max_memory);

        

        while (1) {

                rq = 0;
                total = 0;

                // Request as many blocks as we can
                while (rq < MAX_BLOCKS && total < max_memory) {
                        mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
                        mm_rqs[rq].address = malloc(mm_rqs[rq].size);
                        

                        if (mm_rqs[rq].address) {
                                total += mm_rqs[rq].size;
                                rq++;
                        } else {
                                printf("Error: Couldn't allocate");
                                return;
                        }
                }

                // Set
                uint32_t i;
                for (i = 0; i < rq; i++)
                        if (mm_rqs[i].address)
                                memset(mm_rqs[i].address, i, mm_rqs[i].size);

                // Check
                for (i = 0; i < rq; i++)
                        if (mm_rqs[i].address)
                                if (!memcheck(mm_rqs[i].address, i,
                                                mm_rqs[i].size)) {
                                        printf("test_mm ERROR\n");
                                        return -1;
                                }

                // printf("Memory state with all allocated: \n");

                // commandMem(0, NULL);

                // Free
                for (i = 0; i < rq; i++)
                        if (mm_rqs[i].address)
                                free(mm_rqs[i].address);

                // printf("Memory after FREE: \n");
                // commandMem(0, NULL);
        }


        return 0;
}

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
#ifndef PROCESS_H
#define PROCESS_H

#define PROCESS_NAME_MAX_LEN 64
#define PROCESS_STACK_SIZE 4096
#define MAX_WAITING_COUNT 128

#define READY 1
#define BLOCKED 2
#define KILLED 3

#include <stdint.h>

typedef uint64_t *reg_t;

typedef struct process_t {
        char name[PROCESS_NAME_MAX_LEN];
        uint64_t pid;
        uint64_t ppid;
        reg_t rsp;
        reg_t rbp;
        void *stackPointer;
        uint8_t status;
        uint8_t priority;
        uint64_t waitingCount;
        uint64_t waitingPIDs[MAX_WAITING_COUNT];
        uint64_t sleepingCyclesLeft;
} process_t;

typedef process_t *process;

process createProcess(char *name, uint64_t pid, uint64_t ppid,
                      int (*mainF)(int, char **), int argc, char **argv);
void freeProcess(process p);

#endif /* PROCESS_H */

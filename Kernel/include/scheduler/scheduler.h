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
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <scheduler/process.h>

#define MAX_PROCESSES_INFO 32
#define MAX_PROCESS_PRIORITY 10

uint64_t schedule(uint64_t rsp);

typedef struct processInfo_t {
        char *name;
        unsigned int pid;
        unsigned int ppid;
        unsigned int priority;
        unsigned int status;

} processInfo_t;

typedef struct schInfo_t {
        unsigned int totalReady;
        unsigned int totalProcesses;
        processInfo_t processes[MAX_PROCESSES_INFO];
} schInfo_t;

void initScheduler();
void TEMP_testProcess();
void addProcess(process p);
uint64_t createAndAddProcess(char *name, int (*mainF)(int, char **), int argc,
                             char **argv, uint64_t foreground, uint64_t stdin, uint64_t stdout);
void lockCurrentProcess();
void unlockCurrentProcess();
void unlockProcessByPID(uint64_t pid);
void getSchedulerInfo(schInfo_t *infoBlock);
unsigned int isCurrentProcessForeground();
uint64_t getCurrentProcessPID();
void waitForPID(uint64_t pid);

#endif /* SCHEDULER_H */

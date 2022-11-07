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
#ifndef PROCESS_MANAGEMENT
#define PROCESS_MANAGEMENT
#define STDIN 0
#define STDOUT 1

#include <syscalls_asm.h>
#include <stdio.h>
#include <processManagement.h>

uint64_t createProcess(char *name, int (*mainF)(int, char **), int argc,
                       char **argv, unsigned int foreground)
{
        uint64_t returnPID = 0;
        createProcessSyscall(name, mainF, argc, argv, foreground, STDIN, STDOUT, &returnPID);
        return returnPID;
}

uint64_t createProcessWithFD(char *name, int (*mainF)(int, char **), int argc,
                       char **argv, unsigned int foreground, unsigned int stdin, unsigned int stdout)
{
        uint64_t returnPID = 0;
        createProcessSyscall(name, mainF, argc, argv, foreground, stdin, stdout, &returnPID);
        return returnPID;
}

void waitPID(uint64_t pid)
{
        waitPIDSyscall(pid);
}

void sleep(unsigned int seconds) {
        sleepSyscall(seconds);
}

unsigned int getPid(){
    unsigned int pid;
    getPIDSyscall(&pid);
    return pid;
}

#endif /* PROCESS_MANAGEMENT */

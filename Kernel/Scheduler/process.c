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
#ifndef PROCESS
#define PROCESS

#include <scheduler/process.h>
#include <mem/memory.h>
#include <stdint.h>
#include <regi.h>
#include <naiveConsole.h>
#include <interrupts.h>
#include <utils.h>

static void mainFunctionWrapper(int (*mainF)(int, char **), int argc,
                                char **agrv, uint8_t *processStatus);
static void initStack(reg_t rbp, reg_t rsp,
                      int (*processMainFunction)(int, char **), int argc,
                      char **argv, uint8_t *processStatus);

process createProcess(char *name, uint64_t pid, uint64_t ppid,
                      int (*mainF)(int, char **), int argc, char **argv,
                      uint64_t stdin, uint64_t stdout)
{
        process p = somalloc(sizeof(process_t));
        // ncPrint("###############");
        // ncNewline();
        kernel_strcpy(
                p->name,
                name); //TODO: Assure that the name length is less than NAME_MAX_LEN
        p->pid = pid;
        p->ppid = ppid;
        p->stackPointer = (void *)somalloc(PROCESS_STACK_SIZE);
        p->rbp = (void *)((char *)p->stackPointer + PROCESS_STACK_SIZE -
                          1); //Highest stack address
        p->rsp = (void *)((registerStruct *)p->rbp -
                          1); //rbp + size of stackFrame
        p->status = READY;
        p->sleepingCyclesLeft = 0;
        // Priority goes from 1 to 20. It indicates the total quantums it executes before switching to the next process
        p->priority = 1;
        p->waitingCount = 0;
        p->stdin = stdin;
        p->stdout = stdout;
        for (int i = 0; i < MAX_WAITING_COUNT; i++) {
                p->waitingPIDs[i] = 0;
        }
        initStack(p->rbp, p->rsp, mainF, argc, argv, &(p->status));
        return p;
}

static void initStack(reg_t rbp, reg_t rsp,
                      int (*processMainFunction)(int, char **), int argc,
                      char **argv, uint8_t *processStatus)
{
        registerStruct *stack_frame = (registerStruct *)rsp;
        stack_frame->gs = 0x001;
        stack_frame->fs = 0x002;
        stack_frame->r15 = 0x003;
        stack_frame->r14 = 0x004;
        stack_frame->r13 = 0x005;
        stack_frame->r12 = 0x006;
        stack_frame->r11 = 0x007;
        stack_frame->r10 = 0x008;
        stack_frame->r9 = 0x009;
        stack_frame->r8 = 0x00A;
        stack_frame->rsi = (uint64_t)argc; //
        stack_frame->rdi = (uint64_t)
                processMainFunction; //address of the instruction to "return"
        stack_frame->rbp = 0x00A; // Dummy rbp
        stack_frame->rdx = (uint64_t)argv;
        stack_frame->rcx = (uint64_t)processStatus;
        stack_frame->rbx = 0x010;
        stack_frame->rax = 0x11;
        stack_frame->rip = (uint64_t)mainFunctionWrapper;
        stack_frame->cs = 0x008;
        stack_frame->flags = 0x202;
        stack_frame->rsp = (uint64_t)(&stack_frame->base);
        stack_frame->ss = 0x0;
        stack_frame->base = 0x0;
}

static void mainFunctionWrapper(int (*mainF)(int, char **), int argc,
                                char **argv, uint8_t *processStatus)
{
        mainF(argc, argv);
        *processStatus = KILLED;
        forceTimerTick();
        return;
}

void freeProcess(process p)
{
        sofree(p->stackPointer);
        sofree(p);
}

#endif /* PROCESS */

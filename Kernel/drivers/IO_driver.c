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
#ifndef IO_DRIVER
#define IO_DRIVER

#include <regi.h>
#include <stdint.h>

static registerStruct registerSnapshot;
static uint64_t errNum = 32;

void saveRegisters(registerStruct *registers)
{
        registerSnapshot.r15 = registers->r15;
        registerSnapshot.r14 = registers->r14;
        registerSnapshot.r13 = registers->r13;
        registerSnapshot.r12 = registers->r12;
        registerSnapshot.r11 = registers->r11;
        registerSnapshot.r10 = registers->r10;
        registerSnapshot.r9 = registers->r9;
        registerSnapshot.r8 = registers->r8;
        registerSnapshot.rsi = registers->rsi;
        registerSnapshot.rdi = registers->rdi;
        registerSnapshot.rbp = registers->rbp;
        registerSnapshot.rdx = registers->rdx;
        registerSnapshot.rcx = registers->rcx;
        registerSnapshot.rbx = registers->rbx;
        registerSnapshot.rax = registers->rax;
        registerSnapshot.rip = registers->rip;
        registerSnapshot.cs = registers->cs;
        registerSnapshot.flags = registers->flags;
        registerSnapshot.rsp = registers->rsp;
}

void getRegisters(uint64_t *arr)
{
        uint64_t *regArr = (uint64_t *)&registerSnapshot;
        for (int i = 0; i < TOTAL_REGISTERS; i++) {
                arr[i] = regArr[TOTAL_REGISTERS - 1 - i];
        }
}

void getBytesFromAddress(uint64_t address, uint64_t *target, uint8_t totalBytes)
{
        uint8_t *pos = (uint8_t *)address;
        for (uint8_t i = 0; i < totalBytes; i++) {
                target[i] = (uint64_t)pos[i];
        }
}

void saveErrCode(uint64_t err)
{
        errNum = err;
}

uint64_t getErrCode()
{
        return errNum;
}
#endif /* IO_DRIVER */

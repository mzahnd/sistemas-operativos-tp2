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
#ifndef IDT_LOADER
#define IDT_LOADER

#include <stdint.h>
#include <idtLoader.h>
#include <defs.h>
#include <interrupts.h>

#pragma pack(push)
#pragma pack(1)

typedef struct {
        uint16_t offset_l, selector;
        uint8_t cero, access;
        uint16_t offset_m;
        uint32_t offset_h, other_cero;
} DESCR_INT;

#pragma pack(pop)

DESCR_INT *idt = (DESCR_INT *)0;
static void setup_IDT_entry(int index, uint64_t offset);

void loadIdt()
{
        _cli();
        setup_IDT_entry(0x20, (uint64_t)&_irq00Handler);
        setup_IDT_entry(0x21, (uint64_t)&_irq01Handler);
        setup_IDT_entry(0x80, (uint64_t)&_irq80Handler);
        setup_IDT_entry(0x00, (uint64_t)&_exception00Handler);
        setup_IDT_entry(0x06, (uint64_t)&_exception06Handler);
        picMasterMask(0xFC);
        picSlaveMask(0xFF);
        _sti();
}

static void setup_IDT_entry(int index, uint64_t offset)
{
        idt[index].selector = 0x08;
        idt[index].offset_l = offset & 0xFFFF;
        idt[index].offset_m = (offset >> 16) & 0xFFFF;
        idt[index].offset_h = (offset >> 32) & 0xFFFFFFFF;
        idt[index].access = ACS_INT;
        idt[index].cero = 0;
        idt[index].other_cero = (uint64_t)0;
}

#endif /* IDT_LOADER */

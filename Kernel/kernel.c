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
#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <idtLoader.h>
#include <timer_driver.h>
#include <date_driver.h>
#include <video_driver.h>
#include <keyboard_driver.h>
#include <interrupts.h>
#include <idtLoader.h>
#include <scheduler/scheduler.h>
#include <mem/memory.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void *const sampleCodeModuleAddress = (void *)0x400000; //-V566
static void *const sampleDataModuleAddress = (void *)0x500000; //-V566

typedef int (*EntryPoint)();

void startUserland();

void clearBSS(void *bssAddress, uint64_t bssSize)
{
        somemset(bssAddress, 0, bssSize);
}

void *getStackBase()
{
        return (void *)((uint64_t)&endOfKernel +
                        PageSize * 8 //The size of the stack itself, 32KiB
                        - sizeof(uint64_t) //Begin at the top of the stack
        );
}

void *initializeKernelBinary()
{
        char buffer[10];

        ncPrint("[x64BareBones]");
        ncNewline();

        ncPrint("CPU Vendor:");
        ncPrint(cpuVendor(buffer));
        ncNewline();

        ncPrint("[Loading modules]");
        ncNewline();
        void *moduleAddresses[] = { sampleCodeModuleAddress,
                                    sampleDataModuleAddress };

        loadModules(&endOfKernelBinary, moduleAddresses);
        ncPrint("[Done]");
        ncNewline();
        ncNewline();

        ncPrint("[Initializing kernel's binary]");
        ncNewline();

        clearBSS(&bss, &endOfKernel - &bss);

        // ncPrint("  text: 0x");
        // ncPrintHex((uint64_t)&text);
        // ncNewline();
        // ncPrint("  rodata: 0x");
        // ncPrintHex((uint64_t)&rodata);
        // ncNewline();
        // ncPrint("  data: 0x");
        // ncPrintHex((uint64_t)&data);
        // ncNewline();
        // ncPrint("  bss: 0x");
        // ncPrintHex((uint64_t)&bss);
        // ncNewline();

        ncPrint("[Done]");
        ncNewline();
        ncNewline();
        return getStackBase();
}

int main()
{
        loadIdt();
        initVideoDriver();
        initializeFunctionKeys();
        initializeTickMethods();

        //Test para printmem
        uint8_t *pos = (uint8_t *)0x12345678; //-V566
        for (uint8_t i = 0; i < 32; i++) {
                *(pos + i) = 0x10 + i;
        }

        // ncPrint("[Kernel Main]");
        // ncNewline();
        // ncPrint("  Sample code module at 0x");
        // ncPrintHex((uint64_t)sampleCodeModuleAddress);
        // ncNewline();
        // ncPrint("  Calling the sample code module returned: ");
        saveInitialConditions(sampleCodeModuleAddress);

        initScheduler();
        startUserland();
        // ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
        // ncNewline();
        // ncNewline();

        // ncPrint("  Sample data module at 0x");
        // ncPrintHex((uint64_t)sampleDataModuleAddress);
        // ncNewline();
        // ncPrint("  Sample data module contents: ");
        // ncPrint((char *)sampleDataModuleAddress);
        // ncNewline();

        // ncPrint("[Finished]");

        while (1)
                ;

        return 0;
}

void startUserland()
{
        int r = createAndAddProcess("root", sampleCodeModuleAddress, 0, NULL,
                                    1);
        if (r == 0) {
                ncPrint("ERROR: Could not start Userland");
                ncNewline();
        } else {
                forceTimerTick();
        }
}

// int printA(int argc, char **argv)
// {
//         while (1) {
//                 for (int i = 0; i < 100; i++) {
//                         ncPrint("A");
//                         putProcessToSleep(1);
//                 }
//                 ncClear();
//         }
// }

// int printB(int argc, char **argv)
// {
//         int i = 0;
//         while (i < 2) {
//                 ncPrint("B");
//                 putProcessToSleep(1);
//                 i++;
//         }
//         ncPrint("Done");
// }

// void initTestProcesses()
// {
//         createAndAddProcess("Print A", printA, 0, NULL);
//         createAndAddProcess("Print B", printB, 0, NULL);
// }

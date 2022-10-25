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
#include <semaphore.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;

typedef int (*EntryPoint)();

void initTestProcesses();

int semA(int argc, char **argv);
int semB(int argc, char **argv);
void initTestSem();

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
        uint8_t *pos = (uint8_t *)0x12345678;
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
        /* initTestProcesses(); */
        initTestSem();
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

int printA(int argc, char **argv)
{
        while (1) {
                for (int i = 0; i < 100; i++) {
                        ncPrint("A");
                        putProcessToSleep(1);
                }
                ncClear();
        }
}

int printB(int argc, char **argv)
{
        int i = 0;
        while (i < 2) {
                ncPrint("B");
                putProcessToSleep(1);
                i++;
        }
        ncPrint("Done");
}

void initTestProcesses()
{
        createAndAddProcess("Print A", printA, 0, NULL);
        createAndAddProcess("Print B", printB, 0, NULL);
}

static const uint64_t dummy = 0x0FFFFFFF;
static sosem_t glob_sem;

int semA(int argc, char **argv)
{
        ncPrint("[semA] Hi!");
        ncNewline();
        ncPrint("[semA] About to wait");
        ncNewline();

        sosem_wait(&glob_sem);

        ncPrint("[semA] Critical section");
        ncNewline();

        for (uint64_t i = 0; i < dummy;) {
                i++;
        }

        ncPrint("[semA] About to exit critical section");
        ncNewline();

        sosem_post(&glob_sem);
        ncPrint("[semA] Exited critical section");
        ncNewline();
        return 0;
}

int semB(int argc, char **argv)
{
        ncPrint("[semB] Hi!");
        ncNewline();

        for (uint64_t i = 0; i < dummy;) {
                i++;
        }

        ncPrint("[semB] About to post");
        ncNewline();
        sosem_post(&glob_sem);
        ncPrint("[semB] Posted!");
        ncNewline();

        for (uint64_t i = 0; i < dummy / 4;) {
                i++;
        }

        ncPrint("[semB] About to wait");
        ncNewline();
        sosem_wait(&glob_sem);
        ncPrint("[semB] Critical section");
        ncNewline();

        for (uint64_t i = 0; i < dummy;) {
                i++;
        }
        ncPrint("[semB] About to exit critical section");
        ncNewline();

        sosem_post(&glob_sem);
        ncPrint("[semB] Exited critical section");
        ncNewline();
        return 0;
}

void initTestSem()
{
        int ret = sosem_init(&glob_sem, 0);
        ncPrint("sosem_init() = ");
        ncPrintDec(ret);
        ncNewline();

        createAndAddProcess("semA", semA, 0, NULL);
        createAndAddProcess("semB", semB, 0, NULL);
}

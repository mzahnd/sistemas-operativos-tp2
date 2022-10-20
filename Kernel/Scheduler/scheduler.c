#ifndef SCHEDULER
#define SCHEDULER

#include <scheduler/process.h>
#include <mem/memory.h>
#include <naiveConsole.h>
process current = NULL;


static int scheduler_initialized = 0;

int printA(int argc, char** argv) {
    while(1) {
        // ncPrint("A");
    }
}

void initScheduler() {
    scheduler_initialized = 1;
}

void TEMP_testProcess() {
    ncPrint("Before Create Process -");
    ncNewline();
    current = createProcess("ASD", 100, 1, printA, 0, NULL);
    ncPrint("After Create Process -");
    ncNewline();
}

uint64_t schedule(uint64_t rsp) {
    //ncPrint("CCCCCCCCCCCCCCCCCCCCCCCC");
    if (!scheduler_initialized) {
        return rsp;
    }
    if (current == NULL) {
        return rsp;
    }

    //Guardo el rsp en current
    current->rsp = rsp;

    //Cambio al siguiente;

    //Retorno el RSP de mi current
    return current->rsp;
}


#endif
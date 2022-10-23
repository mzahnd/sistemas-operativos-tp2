#ifndef SCHEDULER
#define SCHEDULER

#include <scheduler/process.h>
#include <scheduler/circularQueue.h>
#include <mem/memory.h>
#include <naiveConsole.h>
#include <interrupts.h>

#define HALT_PROCESS_PID 2

circularQueue queue = NULL;
node currentNode = NULL;

static int scheduler_initialized = 0;
static unsigned int totalReady = 0;
void putProcessToSleep(unsigned int seconds);

int haltProcess(int argc, char** argv) {
    while (1) {
        _hlt();
    }
}

int printA(int argc, char** argv) {
    while(1) {
        for (int i = 0; i < 100; i++) {
            ncPrint("A");
        }
        putProcessToSleep(3);
        ncClear();
    }
}

int printB(int argc, char** argv) {
    while(1) {
        ncPrint("B");
    }
}

void initScheduler() {
    scheduler_initialized = 1;
    queue = newCircularQueue();
    createAndAddProcess("HLT", HALT_PROCESS_PID, 1, haltProcess, 0, NULL);
    totalReady--; // Halt process should not count as ready for the scheduler
}

void TEMP_testProcess() {
    createAndAddProcess("Print A", 100, 1, printA, 0, NULL);
}

uint64_t schedule(uint64_t rsp) {
    if (!scheduler_initialized || queue == NULL || queue->size == 0) {
        return rsp;
    }

    if (!currentNode) {
        currentNode = queue->first;
        return currentNode->pcb->rsp;
    }

    process current = currentNode->pcb;

    //Guardo el rsp en current
    current->rsp = rsp;

    //Cambio al siguiente;
    node nextNode = currentNode->next;
    process next = nextNode->pcb;
    while (next->status != READY || (next->pid == HALT_PROCESS_PID && totalReady > 0)) {

        if (next->sleepingCyclesLeft > 0) {
            next->sleepingCyclesLeft--;
            if (next->sleepingCyclesLeft == 0) {
                next->status = READY;
            }
        }
        nextNode = nextNode->next;
        next = nextNode->pcb;

        if (next->pid == HALT_PROCESS_PID && totalReady != 0) {
            //Halt process should only execute when there is no other process ready
            nextNode = nextNode->next;
            next = nextNode->pcb;
        }
    }
    

    //Retorno el RSP de mi current
    currentNode = nextNode;
    return next->rsp;
}

void addProcess(process p) {
    addToQueue(queue, p);
    totalReady++;
}

void createAndAddProcess(char * name, uint64_t pid, uint64_t ppid, int (*mainF)(int, char**), int argc, char** argv) {
    process p = createProcess(name, pid, ppid, mainF, argc, argv);
    addToQueue(queue, p);
    totalReady++;
}

void putProcessToSleep(unsigned int seconds) {
    if (!scheduler_initialized || currentNode == NULL) {
        return;
    }
    process p = currentNode->pcb;
    if (p == NULL) {
        return;
    }
    
    if (p->status == BLOCKED) { //Cannot put to sleep a blocked process
        return;
    }
    int totalCycles = (unsigned int)(seconds/0.055);
    p->sleepingCyclesLeft = totalCycles;
    p->status = BLOCKED;
    totalReady--;
    forceTimerTick();
}

#endif
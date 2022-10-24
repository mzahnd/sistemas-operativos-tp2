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
static unsigned int maxPID = 100;
void putProcessToSleep(unsigned int seconds);

int haltProcess(int argc, char **argv)
{
        while (1) {
                _hlt();
        }
}

void initScheduler()
{
        scheduler_initialized = 1;
        queue = newCircularQueue();
        createAndAddProcess("HLT", haltProcess, 0, NULL);
        totalReady--; // Halt process should not count as ready for the scheduler
}

uint64_t schedule(uint64_t rsp)
{
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
        while (next->status != READY ||
               (next->pid == HALT_PROCESS_PID && totalReady > 0)) {
                if (nextNode == currentNode &&
                    currentNode->pcb->status != KILLED) {
                        //Means that the queue was completely walked and only one process is active
                        return current->rsp;
                }

                if (next->status == KILLED) {
                        node nodeToKill = nextNode;
                        nextNode = nextNode->next;
                        next = nextNode->pcb;
                        removeFromQueue(queue, nodeToKill->pcb->pid);
                        freeProcess(nodeToKill->pcb);
                        totalReady--;
                        continue;
                }

                if (next->sleepingCyclesLeft >
                    0) { // Only gets here is status is BLOCKED
                        next->sleepingCyclesLeft--;
                        if (next->sleepingCyclesLeft == 0) {
                                next->status = READY;
                                totalReady++;
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

void addProcess(process p)
{
        addToQueue(queue, p);
        totalReady++;
}

void createAndAddProcess(char *name, int (*mainF)(int, char **), int argc,
                         char **argv)
{
        if (!scheduler_initialized || queue == NULL) {
                return;
        }

        uint64_t pid;
        if (mainF == haltProcess) {
                pid = 2;
        } else {
                pid = maxPID;
                maxPID++;
        }

        uint64_t ppid;
        if (currentNode == NULL) {
                ppid = 1;
        } else {
                ppid = currentNode->pcb->pid;
        }

        process p = createProcess(name, pid, ppid, mainF, argc, argv);
        addToQueue(queue, p);
        totalReady++;
}

void putProcessToSleep(unsigned int seconds)
{
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
        int totalCycles = (unsigned int)(seconds / 0.055);
        p->sleepingCyclesLeft = totalCycles;
        lockCurrentProcess();
}

void lockCurrentProcess()
{
        if (!scheduler_initialized || currentNode == NULL) {
                return;
        }
        process p = currentNode->pcb;
        if (p == NULL || p->status == BLOCKED || p->status == KILLED) {
                return;
        }
        p->status = BLOCKED;
        totalReady--;
        forceTimerTick();
}

void unlockCurrentProcess()
{
        if (!scheduler_initialized || currentNode == NULL) {
                return;
        }
        process p = currentNode->pcb;
        if (p == NULL || p->status == READY || p->status == KILLED) {
                return;
        }
        p->status = READY;
        totalReady++;
}

void unlockProcessByPID(uint64_t pid)
{
        if (!scheduler_initialized || queue == NULL || queue->size == NULL) {
                return;
        }
        int init = 0;
        node current = queue->first;
        node first = current;
        while (current != first && init) {
                init = 1;
                process p = current->pcb;
                if (p->pid == pid) {
                        if (p->status == KILLED || p->status == READY) {
                                return;
                        }
                        p->status = READY;
                        totalReady++;
                        return;
                }
                current = current->next;
        }
}

#endif
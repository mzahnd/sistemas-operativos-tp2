#ifndef SCHEDULER
#define SCHEDULER

#include <scheduler/process.h>
#include <scheduler/circularQueue.h>
#include <scheduler/scheduler.h>
#include <mem/memory.h>
#include <naiveConsole.h>
#include <interrupts.h>

#define HALT_PROCESS_PID 2
#define KERNEL_PROCESS_PID 1

circularQueue queue = NULL;
node currentNode = NULL;

static int scheduler_initialized = 0;
static unsigned int totalReady = 0;
static unsigned int maxPID = 100;
static unsigned int currentProcessCycle = 0;
static unsigned int foregroundProcessPID = KERNEL_PROCESS_PID;

void putProcessToSleep(unsigned int seconds);
void unlockWaitingProcesses(process p);

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
        createAndAddProcess("___HLT___", haltProcess, 0, NULL, 0);
        totalReady--; // Halt process should not count as ready for the scheduler
}

uint64_t schedule(uint64_t rsp)
{
        if (!scheduler_initialized || queue == NULL || queue->size == 0) {
                return rsp;
        }

        if (!currentNode) {
                currentNode = queue->first;
                return (uint64_t)currentNode->pcb->rsp;
        }

        process current = currentNode->pcb;

        //Guardo el rsp en current
        current->rsp = (reg_t)rsp;

        if (current->priority > currentProcessCycle &&
            current->status != KILLED && current->status != BLOCKED) {
                currentProcessCycle++;
                return (uint64_t)current->rsp;
        }

        //Cambio al siguiente;
        node nextNode = currentNode->next;
        process next = nextNode->pcb;
        while (next->status != READY ||
               (next->pid == HALT_PROCESS_PID && totalReady > 0)) {
                if (nextNode == currentNode &&
                    currentNode->pcb->status != KILLED) {
                        //Means that the queue was completely checked and only one process is active
                        break;
                }

                if (next->status == KILLED) {
                        node nodeToKill = nextNode;
                       	if (nodeToKill->pcb->pid == foregroundProcessPID) {
				foregroundProcessPID = nodeToKill->pcb->ppid;
			}
			unlockWaitingProcesses(nodeToKill->pcb);
			nextNode = nextNode->next;
                        next = nextNode->pcb;
                        removeFromQueue(
                                queue,
                                nodeToKill->pcb
                                        ->pid); // Remove makes the free of the process

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
        currentProcessCycle = 0;
        return (uint64_t)next->rsp;
}

void addProcess(process p)
{
        addToQueue(queue, p);
        totalReady++;
}

uint64_t createAndAddProcess(char *name, int (*mainF)(int, char **), int argc,
                         char **argv, uint64_t foreground)
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
        if (currentNode == NULL || currentNode->pcb->pid == HALT_PROCESS_PID) {
                ppid = KERNEL_PROCESS_PID;
        } else {
                ppid = currentNode->pcb->pid;
        }

        if (ppid == foregroundProcessPID && foreground) {
                foregroundProcessPID = pid;
        }

        process p = createProcess(name, pid, ppid, mainF, argc, argv);
        addToQueue(queue, p);
        totalReady++;
        return pid;
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
        if (p->status == READY) {
                p->status = BLOCKED;
                totalReady--;
        }
        currentProcessCycle = MAX_PROCESS_PRIORITY + 1; // To force the context Switch
        _sti();
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
        if (!scheduler_initialized || queue == NULL || queue->size == 0) {
                return;
        }
        int init = 0;
        node current = queue->first;
        node first = current;
        while (current != first || !init) {
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

void getSchedulerInfo(schInfo_t * infoBlock) {
        if (!scheduler_initialized || queue == NULL || currentNode == NULL) {
                return;
        }
        
        infoBlock->totalReady = totalReady;
        infoBlock->totalProcesses = queue->size;
        node infoNode = queue->first;
        for (int i = 0; i < MAX_PROCESSES_INFO && i < queue->size; i++, infoNode=infoNode->next) {
                processInfo_t * processInfo = &(infoBlock->processes[i]);
                process p = infoNode->pcb;
                processInfo->name = p->name;
                processInfo->pid = p->pid;
                processInfo->ppid = p->ppid;
                processInfo->priority = p->priority;
                processInfo->status = p->status;
        }
}

uint64_t getCurrentProcessPID()
{
        if (!scheduler_initialized || currentNode == NULL) {
                return 0;
        }
        process p = currentNode->pcb;
        if (p == NULL || p->status == KILLED) {
                return 0;
        }
        return p->pid;
}

unsigned int isCurrentProcessForeground() {
        if (!scheduler_initialized || queue == NULL || queue->size == 0) {
                return 0; 
        }
        return currentNode->pcb->pid == foregroundProcessPID;
}

void waitForPID(uint64_t pid) {
	if (!scheduler_initialized || queue == NULL || queue->size == 0) {
		return;
	}
	process p = getFromPID(queue, pid);
	if (p == NULL) {
		return;
	}
	if (p->waitingCount < MAX_WAITING_COUNT) {
		p->waitingPIDs[p->waitingCount] = currentNode->pcb->pid;
		p->waitingCount++;
		lockCurrentProcess();
	}
}

void unlockWaitingProcesses(process p) {
	if (p == NULL) {
		return;
	}
	for (int i = 0; i < p->waitingCount; i++) {
		unlockProcessByPID(p->waitingPIDs[i]);
	}
}


#endif

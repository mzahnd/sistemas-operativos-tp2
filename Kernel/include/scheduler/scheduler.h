#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <scheduler/process.h>

#define MAX_PROCESSES_INFO 32

uint64_t schedule(uint64_t rsp);

typedef struct processInfo_t {
    unsigned char * name;
    unsigned int pid;
    unsigned int ppid;
    unsigned int priority;
    unsigned int status;
    
} processInfo_t;

typedef struct schInfo_t {
    unsigned int totalReady;
    unsigned int totalProcesses;
    processInfo_t processes[MAX_PROCESSES_INFO];
}  schInfo_t;


void initScheduler();
void TEMP_testProcess();
void addProcess(process p);
void createAndAddProcess(char *name, int (*mainF)(int, char **), int argc,
                         char **argv);
void lockCurrentProcess();
void unlockCurrentProcess();
void unlockProcessByPID(uint64_t pid);
void getSchedulerInfo(schInfo_t * infoBlock);
uint64_t getCurrentProcessPID();

#endif

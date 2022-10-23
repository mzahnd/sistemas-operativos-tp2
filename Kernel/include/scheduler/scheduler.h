#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <scheduler/process.h>

uint64_t schedule(uint64_t rsp);
void initScheduler();
void TEMP_testProcess();
void addProcess(process p);
void createAndAddProcess(char * name, int (*mainF)(int, char**), int argc, char** argv);
void lockCurrentProcess();
void unlockCurrentProcess();
void unlockProcessByPID(uint64_t pid);


#endif
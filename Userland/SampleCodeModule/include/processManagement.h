#ifndef PROCESS_MANAGEMENT_H
#define PROCESS_MANAGEMENT_H
#include <stdint.h>

uint64_t createProcess(char *name, int (*mainF)(int, char **), int argc,
                       char **argv, unsigned int foreground);
int waitPID(uint64_t pid);
#endif
#ifndef PROCESS_MANAGEMENT
#define PROCESS_MANAGEMENT
#include <syscalls_asm.h>
#include <processManagement.h>

void createProcess(char * name, int (*mainF)(int, char**), int argc, char** argv, unsigned int foreground) {
    createProcessSyscall(name, mainF, argc, argv, foreground);
}

#endif
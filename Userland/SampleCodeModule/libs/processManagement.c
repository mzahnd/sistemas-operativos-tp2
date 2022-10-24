#ifndef PROCESS_MANAGEMENT
#define PROCESS_MANAGEMENT
#include <syscalls_asm.h>

void createProcess(char * name, int (*mainF)(int, char**), int argc, int argv) {
    createProcessSyscall(name, mainF, argc, argv);
}

#endif
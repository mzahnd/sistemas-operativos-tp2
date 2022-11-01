#ifndef PROCESS_MANAGEMENT
#define PROCESS_MANAGEMENT
#include <syscalls_asm.h>
#include <processManagement.h>

uint64_t createProcess(char *name, int (*mainF)(int, char **), int argc,
                       char **argv, unsigned int foreground)
{
        uint64_t returnPID = 0;
        createProcessSyscall(name, mainF, argc, argv, foreground, &returnPID);
        return returnPID;
}

int waitPID(uint64_t pid)
{
        waitPIDSyscall(pid);
}

#endif
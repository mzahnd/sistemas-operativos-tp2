#include <processes.h>
#include <stdio.h>
#include <syscalls_asm.h>

void processKiller(){
    int pid;
    getPIDSyscall(&pid);
    
    killProcessSyscall(pid);
    while (1);
}

int getPid(){
    int pid;
    getPIDSyscall(&pid);
    return pid;
}





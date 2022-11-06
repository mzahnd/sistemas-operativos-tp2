#include <stdio.h>

void processKiller(){
    int pid;
    getPIDSyscall(&pid);
    int ans;
    killSyscall(pid,&ans);
    while (1);
}

int getPid(){
    int pid;
    getPIDSyscall(&pid);
    return pid;
}





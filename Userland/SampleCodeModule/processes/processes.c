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

int nice(int pid, int prio){
    int ans;
    niceSyscall(pid, prio, &ans);
    return ans;
}

int block(int pid) {
    int ans;
    blockSyscall(pid, &ans); //falta syscall
    return ans;
}

int unblock(int pid) {
    int ans;
    unblockSyscall(pid, &ans); // falta syscall
    return ans;
}

int kill(int pid){
    int ans;
    killSyscall(pid, &ans);
    return ans;
}
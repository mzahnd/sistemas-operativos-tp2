#include <stdio.h>

void processKiller(){
    uint64_t pid;
    getPIDSyscall(&pid);
    int ans;
    killSyscall(pid,&ans);
    while (1);
}

uint64_t getPid(){
    uint64_t pid;
    getPIDSyscall(&pid);
    return pid;
}

int nice(uint64_t pid, uint64_t prio){
    int ans;
    niceSyscall(pid, prio, &ans);
    return ans;
}

int block(uint64_t pid) {
    int ans;
    blockSyscall(pid, &ans); //falta syscall
    return ans;
}

int unblock(uint64_t pid) {
    int ans;
    unblockSyscall(pid, &ans); // falta syscall
    return ans;
}

int kill(uint64_t pid){
    int ans;
    killSyscall(pid, &ans);
    return ans;
}
#include <processes.h>
#include <syscalls_asm.h>
#include <stdio.h>

static int nice(int pid, int prio){
    int ans;
    niceSyscall(pid, prio, &ans);
    return ans;
}

int commandNice(int argc, char **argv) {
    putChar('\n');
    int id = atoi(argv[1]);
    int priority = atoi(argv[2]);
    if(id < 0 || priority <= 0) {
        printf("Invalid arguments\n");
        return -1;
    }
    if(nice(id, priority) < 0) {
        printf("Priority change failed\n");
    } else {
        printf("Priority successfully changed\n");
    }
    
    return 1;
}

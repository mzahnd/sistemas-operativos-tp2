#include <processes.h>
#include <syscalls_asm.h>
#include <stdio.h>
#include <stdlib.h>

// static int nice(int pid, int prio){
//     int ans;
//     changeProcessPrioritySyscall(pid, prio);
//     return ans;
// }

int commandNice(int argc, char **argv)
{
        int id = atoi(argv[1]);
        int priority = atoi(argv[2]);
        if (id < 0 || priority <= 0) {
                printf("Invalid arguments\n");
                return -1;
        }
        changeProcessPrioritySyscall(id, priority);
        // if(nice(id, priority) < 0) {
        //     printf("Priority change failed\n");
        // } else {
        //     printf("Priority successfully changed\n");
        // }

        return 0;
}

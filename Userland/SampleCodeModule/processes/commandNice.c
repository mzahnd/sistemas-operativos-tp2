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
        if (argc != 3) {
                printf("ERROR: PID and new priority required\n");
        }

        int pid = atoi(argv[1]);
        int priority = atoi(argv[2]);
        if (pid <= 0) {
                printf("ERROR: Invalid PID\n");
        }
        if (priority > MAX_PROCESS_PRIORITY || priority <= 0) {
                printf("ERROR: Proiority is bounded between 1 and %d\n",
                       MAX_PROCESS_PRIORITY);
        }
        changeProcessPrioritySyscall(pid, priority);
        // if(nice(id, priority) < 0) {
        //     printf("Priority change failed\n");
        // } else {
        //     printf("Priority successfully changed\n");
        // }

        return 0;
}

#include <processes.h>
#include <syscalls_definitions.h>
#include <syscalls_asm.h>
#include <stdio.h>
#include <stdlib.h>

int schedulerInfo(int argc, char **argv)
{
        schInfo_t info;
        getSchedulerInfoSyscall(&info);
        printf("\n");
        printf("NAME\tPID\tPPID\tSTATUS\n");
        for (int i = 0; i < info.totalProcesses; i++) {
                char *status;
                if (info.processes[i].status == 1) {
                        status = "READY";
                } else if (info.processes[i].status == 2) {
                        status = "BLOCKED";
                } else if (info.processes[i].status == 3) {
                        status = "KILLED";
                }
                printf("%s\t%d\t%d\t%s\n", info.processes[i].name,
                       info.processes[i].pid, info.processes[i].ppid, status);
        }

        return 0;
}
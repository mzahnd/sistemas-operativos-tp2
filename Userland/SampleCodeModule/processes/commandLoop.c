#include <processes.h>

int commandLoop(int argc, char **argv) {
    unsigned int pid = getPid();
    printf('\n');
    
    while(1) {
        printf("Hi! You ran the loop command, the PID is: %d. To quit the loop, press F5\n", pid);

        int ticksEnd = getTicks() + 5;
        while(getTicks() < ticksEnd) {
            ;
        }
    }

    return 1;
}
#include <processes.h>

int commandUnblock(int argc, char **argv) {
    putChar('\n');
    unsigned int id = atoi(argv[1]);
    if(unblock(id) == 0) {
        printf("Process successfully unblocked\n");
    } else {
        printf("Process failed to unblock\n");
    }
    return 1;
}
#include <processes.h>
#include <syscalls_asm.h>

int commandPs(int argc, char **argv) {
    putChar('\n');
    schInfo_t *info;
    /* psSyscall(info); */ 
    printf("%s\n", info);
    return 1;
}

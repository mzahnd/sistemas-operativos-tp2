#include <processes.h>
#include <syscalls_asm.h>

int commandMem(int argc, char **argv) {
    putChar('\n');
    int strSize = 2048;
    char str[strSize];
    memSyscall(str, strSize); 
    printf("%s\n", str);

    return 1;
}

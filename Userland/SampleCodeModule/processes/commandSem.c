#include <processes.h>
#include <syscalls_asm.h>

int commandSem(int argc, char **argv) {
    putChar('\n');
    int strSize = 2048;
    char str[strSize];
    semaphoreGetInformationSyscall(str, strSize); //cambiar
    printf("%s\n", str);
    return 1;
}
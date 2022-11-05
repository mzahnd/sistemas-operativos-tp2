#include <processes.h>
#include <syscalls_asm.h>

int commandSem(int argc, char **argv) {
    putChar('\n');
    sem_t *sem;
    unsigned int *restrict sval;
    int *toReturn;
    semaphoreGetInformationSyscall(sem, sval, toReturn); //cambiar
    printf("%s\n", toReturn->value);
    return 1;
}
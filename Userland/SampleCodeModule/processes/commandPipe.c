#include <processes.h>
#include <syscalls_asm.h>

int commandPipe(int argc, char **argv) {
    putChar('\n');
    pipe_info_t **toReturn;
    pipeGetInformationSyscall(2, toReturn); 
    printf("%s\n", toReturn.buffer);
    // como obtener la info del buffer del toReturn
    return 1;
}

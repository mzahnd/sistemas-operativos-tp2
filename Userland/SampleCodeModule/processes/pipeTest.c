#include <processes.h>
#include <stdio.h>
#include <syscalls_asm.h>

int fds[2];

int testPipes(int argc, char** argv) {
    int result = 1234;
    pipePipeSyscall(fds, &result);
    printf("fds = {%d, %d}\n", fds[0], fds[1]);
    pipeCloseSyscall(fds[0], &result);
    pipeCloseSyscall(fds[1], &result);
}
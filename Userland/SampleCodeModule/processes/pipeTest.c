#include <processes.h>
#include <stdio.h>
#include <syscalls_asm.h>

int fds[2];

int testWrite(int argc, char **argv)
{
        char buf[32] = "Hello world!";
        ssize_t s = 0;

        printf("[testWrite] OK. PIPE_FD_WRITE = %d\n", fds[PIPE_FD_WRITE]);

        s = write(fds[PIPE_FD_WRITE], buf, 13);
        printf("[Call] %s = write(fd = %d, %s, 13)\n", s, fds[PIPE_FD_WRITE],
               buf);

        printf("Wrote: %s\n", buf);

        return 0;
}

int testRead(int argc, char **argv)
{
        char buf[32] = { 0 };
        ssize_t s = 0;

        printf("[testRead] OK. PIPE_FD_READ = %d\n", fds[PIPE_FD_READ]);
        s = read(fds[PIPE_FD_READ], buf, 32);
        printf("[Call] %s = read(fd = %d, buf, 32)\n", s, fds[PIPE_FD_READ]);

        printf("Read: %s\n", buf);

        return 0;
}

int testPipes(int argc, char **argv)
{
        int result = 1234;
        result = pipe(fds);
        printf("[Call] %d = pipe(%p)\n", result, fds);

        printf("fds = {%d, %d}\n", fds[0], fds[1]);

        // result = close(fds[0]);
        // printf("[Call] %d = close(%d)\n", result, fds[0]);

        // result = close(fds[1]);
        // printf("[Call] %d = close(%d)\n", result, fds[1]);

        return 0;
}

#include <pipeUser.h>

void pipe_open(int fd)
{
        pipeOpenSyscall(fd);
}

void pipe_close(int fd)
{
        pipeCloseSyscall(fd);
}

void pipe_read(int fd, char *buf, size_t count)
{
        pipeReadSyscall(fd, buf, count);
}

void pipe_write(int fd, const char *buf, size_t count)
{
        pipeWriteSyscall(fd, buf, count);
}
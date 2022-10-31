#ifndef PIPE_USER_H
#define PIPE_USER_H

#include <syscalls_asm.h>
#include <stdint.h>

void pipe_open(int fd);
void pipe_close(int fd);
void pipe_read(int fd, char *buf, size_t count);
void pipe_write(int fd, const char *buf, size_t count);

#endif
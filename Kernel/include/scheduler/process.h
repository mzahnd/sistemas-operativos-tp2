#ifndef PROCESS
#define PROCESS
#define NAME_MAX_LEN 64

#include <stdint.h>

typedef uint64_t * reg_t;

typedef struct process_t
{
    char name[NAME_MAX_LEN];
    uint64_t pid;
    uint64_t ppid;
    reg_t rsp;
    reg_t rbp;
    void * stackPointer;
} process_t;

typedef process_t * process;


#endif
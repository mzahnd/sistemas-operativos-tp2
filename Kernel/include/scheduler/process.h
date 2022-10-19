#ifndef PROCESS_H
#define PROCESS_H
#define PROCESS_NAME_MAX_LEN 64
#define PROCESS_STACK_SIZE 4096

#include <stdint.h>

typedef uint64_t * reg_t;

typedef struct process_t
{
    char name[PROCESS_NAME_MAX_LEN];
    uint64_t pid;
    uint64_t ppid;
    reg_t rsp;
    reg_t rbp;
    void * stackPointer;
} process_t;

typedef process_t * process;

process createProcess(char * name, uint64_t pid, uint64_t ppid, int(* mainF)(int, char**), int argc, char** argv);


#endif
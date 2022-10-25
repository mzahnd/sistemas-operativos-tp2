#ifndef SYSCALL_DEF_H
#define SYSCALL_DEF_H

#define MAX_PROCESSES_INFO 32

typedef struct processInfo_t {
    unsigned char * name;
    unsigned int pid;
    unsigned int ppid;
    unsigned int priority;
    unsigned int status;
    
} processInfo_t;

typedef struct schInfo_t {
    unsigned int totalReady;
    unsigned int totalProcesses;
    processInfo_t processes[MAX_PROCESSES_INFO];
}  schInfo_t;
#endif
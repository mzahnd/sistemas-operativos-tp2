// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**
 * This file is part of sistemas-operativos-tp2
 * Licensed under BSD 3-Clause "New" or "Revised" License.
 * Copyright (c) 2022 Flores Levalle, M.
 *                    López, P.
 *                    Sierra Pérez, C.
 *                    Zahnd, M. E.
 */
#ifndef SYSCALLS_ASM_H
#define SYSCALLS_ASM_H

#include <stddef.h> /* size_t */
#include <stdint.h>
#include <syscalls_definitions.h>
#include <semaphoreUser.h> /* sem_t, sem_info_t */
#include <pipeUser.h> /* pipe_info_t */

void drawStringSysCall(char *buff, uint64_t size, uint64_t x, uint64_t y,
                       uint64_t fontColor, uint64_t backgroundColor,
                       uint64_t fontSize, uint64_t alphaBackground);
void isKeyboardEmptySyscall(uint64_t *target);
void readKeyboardSysCall(int *buffer, uint64_t size, uint64_t *count);
void clearDisplaySyscall(uint64_t color);
void drawRectSyscall(uint64_t xi, uint64_t yi, uint64_t width, uint64_t height,
                     uint64_t color);
void setFunctionKeyMethodSyscall(uint64_t index, void (*)());
void getTimeSyscall(uint8_t mode, uint8_t *target);
void getTicksSyscall(uint64_t *ticks);
void getRegistersSyscall(uint64_t *registers);
void getMemSyscall(uint64_t, uint64_t *, uint8_t);

void readErrorSyscall(uint64_t *err);
void setTimerFunctionSyscall(int index, int tickInterval, void (*)());
void deleteTimerFunctionSyscall(int index);
void mallocSyscall(size_t size, void **result);
void callocSyscall(size_t nmemb, size_t size, void **result);
void freeSyscall(void *ptr);
void memGetInformationSyscall(mem_info_t **result);
void createProcessSyscall(char *name, int (*mainFunction)(int, char **),
                          int argc, char **argv, uint64_t foreground,
                          uint64_t stdin, uint64_t stdout,
                          uint64_t *returnedPID);
void getSchedulerInfoSyscall(schInfo_t *);
void waitPIDSyscall(uint64_t pid);

void pipeCloseSyscall(int fd, int *toReturn);
void pipeGetInformationSyscall(pipe_info_t *restrict last,
                               pipe_info_t **toReturn);
void pipePipeSyscall(int fildes[PIPE_N_FD], int *toReturn);
void pipeReadSyscall(int fd, char *buf, size_t count, ssize_t *toReturn);
void pipeWriteSyscall(int fd, const char *buf, size_t count, ssize_t *toReturn);

void semaphoreCloseSyscall(sem_t *sem, int *toReturn);
void semaphoreDestroySyscall(sem_t *sem, int *toReturn);
void semaphoreGetInformationSyscall(sem_info_t *restrict sem,
                                    sem_info_t **toReturn);
void semaphoreGetValueSyscall(sem_t *sem, unsigned int *restrict sval,
                              int *toReturn);
void semaphoreInitSyscall(sem_t *sem, unsigned int initialValue, int *toReturn);
void semaphoreOpenSyscall(const char *name, unsigned int initialValue,
                          sem_t **toReturn);
void semaphorePostSyscall(sem_t *sem, int *toReturn);
void semaphoreWaitSyscall(sem_t *sem, int *toReturn);

void getCurrentProcessFDSyscall(int fd[2]);
void niceSyscall(unsigned int pid, unsigned int priority, int *result);

void getPIDSyscall(unsigned int *);

void changeProcessPrioritySyscall(unsigned int pid, unsigned int newPriority);
void changeProcessStatusSyscall(unsigned int pid);
void killProcessSyscall(unsigned int pid);
void sleepSyscall(unsigned int seconds);
void giveUpCPUSyscall();

#endif /* SYSCALLS_ASM_H */

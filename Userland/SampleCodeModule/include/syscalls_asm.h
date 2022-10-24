#ifndef SYSCALLS_ASM_H
#define SYSCALLS_ASM_H

#include <stdint.h>

void drawStringSysCall(char *buff, uint64_t size, uint64_t x, uint64_t y,
                       uint64_t fontColor, uint64_t backgroundColor,
                       uint64_t fontSize, uint64_t alphaBackground);
void isKeyboardEmptySyscall(uint64_t *target);
void readKeyboardSysCall(char *buffer, uint64_t size, uint64_t *count);
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
void createProcessSyscall(char * name, int (*mainFunction)(int, char**), int argc, char** argv);

#endif

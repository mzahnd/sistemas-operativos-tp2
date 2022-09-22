#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#define CTRL_L 29
#define SHIFT_L 42
#define SHIFT_R 54

#define BLOCK_M 58

#define KEY_RELEASED 128
#define KEYS 59

#define F1 59
#define F12 88

#define KEY_UP 72
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_DOWN 80

#include <regi.h>
#include <stdint.h>

void keyboardHandler(registerStruct *);
void readKeyboard(char *buff, uint64_t size, uint64_t *count);
void bufferEmpty(uint64_t *target);
void isMayus(uint64_t *target);
void initializeFunctionKeys();
void setFunctionKeyMethod(uint64_t index, void (*function)());
#endif

#ifndef STD_IO_H
#define STD_IO_H value

#include <stdint.h>

#define KEY_UP (char)200
#define KEY_LEFT (char)203
#define KEY_RIGHT (char)205
#define KEY_DOWN (char)208

void scanf(char * buffer);
void printf(char * fmt, ...);
void setConsoleUpdateFunction(void (*f)(char *, int));
void putChar(char ch);
void setFunctionKey(int index, void(*func)());
char getChar();
uint64_t getError();
void format(char *str, int value);

#endif

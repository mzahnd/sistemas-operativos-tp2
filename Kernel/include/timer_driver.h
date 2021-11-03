#ifndef TIME_DRIVER_H
#define TIME_DRIVER_H

#include <stdint.h>

void timeHandler();
uint64_t getTicks();
int secondsElapsed();
void initializeTickMethods();
void setTickMethod(int methodIndex, int tickInterval, void (*function)());
void deleteTickMethod(int methodIndex);

#endif

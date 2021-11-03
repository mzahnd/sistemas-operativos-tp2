#ifndef TIME_H
#define TIME_H

#include <stdint.h>

int getSeconds();
int getMinutes();
int getHours();
int getDays();
int getMonth();
int getYear();
int getTicks();
void setTimerFunction(int index, int tickInterval, void(*)());

#endif

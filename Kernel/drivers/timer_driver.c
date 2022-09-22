#ifndef TIME_DRIVER
#define TIME_DRIVER

#include <stdint.h>
#include <timer_driver.h>
#define TOTAL_METHODS 16

static void (*ticksMethods[TOTAL_METHODS])(void);
static int intervalTicks[TOTAL_METHODS];

static uint64_t ticks = 0;

void timeHandler()
{
        ticks++;
        for (int i = 0; i < TOTAL_METHODS; i++) {
                if (intervalTicks[i] > 0 && ticks % intervalTicks[i] == 0) {
                        (*ticksMethods[i])();
                }
        }
}

uint64_t getTicks()
{
        return ticks;
}

void initializeTickMethods()
{
        for (int i = 0; i < TOTAL_METHODS; i++) {
                ticksMethods[i] = 0;
                intervalTicks[i] = 0;
        }
}

int secondsElapsed()
{
        return ticks / 18;
}

void setTickMethod(int methodIndex, int tickInterval, void (*function)())
{
        if (methodIndex < TOTAL_METHODS) {
                intervalTicks[methodIndex] = tickInterval;
                ticksMethods[methodIndex] = function;
        }
}

void deleteTickMethod(int methodIndex)
{
        if (methodIndex < TOTAL_METHODS) {
                intervalTicks[methodIndex] = 0;
                ticksMethods[methodIndex] = 0;
        }
}

#endif

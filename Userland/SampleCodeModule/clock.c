#ifndef CLOCK
#define CLOCK
#include <time.h>
#include <stdGraphics.h>
#include <colors.h>
#include <stdio.h>
#include <clock.h>

void drawClock()
{
        char str[9] = "00:00:00";
        char hours[] = "00";
        char minutes[] = "00";
        char seconds[] = "00";
        format(hours, getHours());
        format(minutes, getMinutes());
        format(seconds, getSeconds());
        str[0] = hours[0];
        str[1] = hours[1];
        str[3] = minutes[0];
        str[4] = minutes[1];
        str[6] = seconds[0];
        str[7] = seconds[1];

        drawRect(0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, LIGHT_GRAY);
        drawString(SCREEN_WIDTH / 4 - 4 * BASE_CHAR_WIDTH * 2,
                   SCREEN_HEIGHT / 4 - BASE_CHAR_HEIGHT, str, 9, WHITE,
                   LIGHT_GRAY, 2, 1);
}

#endif

#ifndef CLOCK
#define CLOCK
#include <time.h>
#include <stdGraphics.h>
#include <colors.h>
#include <clock.h>
static void format(char *str, int value) {
    if (value < 10) {
        str[1] = value + '0';
    } else {
        str[0] = (value / 10) + '0';
        str[1] = (value % 10) + '0';
    }
    str[2] = 0;
}

void drawClock() {
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

  drawRect(0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, LIGHT_GRAY);
  drawString(100, 100, str, 9, WHITE, LIGHT_GRAY, 2, 1);
}


#endif

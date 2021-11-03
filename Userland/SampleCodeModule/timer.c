#ifndef TIMER
#define TIMER
#define MILISECONDS_PER_TICK 55
#include <time.h>
#include <stdio.h>
#include <stdGraphics.h>
#include <colors.h>
//#include <timer.h>

static long init_ticks = 0;

void startTimer() {
  long init_ticks = getTicks();
}

void drawTimer() {
  long current_ticks = getTicks();
  long total_ticks = current_ticks - init_ticks;
  long total_mil = total_ticks * MILISECONDS_PER_TICK;
  int totalCent = (total_mil % 1000)/10;
  int totalSec = (total_mil % 60000)/1000;
  int totalMin = (total_mil % 3600000)/60000;
  char minutes[] = "00";
  char seconds[] = "00";
  char cent[] = "00";
  format(minutes, totalMin);
  format(seconds, totalSec);
  format(cent, totalCent);
  char str[9] = "00:00:00";
  str[0] = minutes[0];
  str[1] = minutes[1];
  str[3] = seconds[0];
  str[4] = seconds[1];
  str[6] = cent[0];
  str[7] = cent[1];
  drawRect(SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, BLACK);
  drawString(SCREEN_WIDTH/2 + SCREEN_WIDTH/4 - 4 * BASE_CHAR_WIDTH * 2, SCREEN_HEIGHT/4 - BASE_CHAR_HEIGHT, str, 9, YELLOW, BLACK, 2, 1);
}


#endif

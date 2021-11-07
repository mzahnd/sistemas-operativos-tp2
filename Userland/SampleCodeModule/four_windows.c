#ifndef FOUR_WINDOWS
#define FOUR_WINDOWS
#include <timer.h>
#include <clock.h>
#include <time.h>
#include <stdio.h>
#include <four_windows.h>
#include <shells.h>
#include <ahorcado.h>
#include <sudoku.h>

static int running = 0;

void init_apps() {
  running = 1;
  setTimerFunction(0, 18, drawClock);
  setTimerFunction(1, 3, drawTimer);
  setFunctionKey(1, stop_apps);
  drawClock();
  init_sudoku();
  init_ahorcado();
  startTimer();
  draw_sudoku();
  draw_background();
  draw_lives();
  draw_word();
  while(running) {
    char ch = getChar();
    if (keyPressedSudoku(ch)) {
      draw_sudoku();
      highlightCurrent();
    } else {
      keyPressedAhorcado(ch);
    }
  }
  drawShellLines();
}

void stop_apps() {
  running = 0;
  deleteTimerFunction(0);
  deleteTimerFunction(1);
  drawShellLines();
  putChar('\n');
}

#endif

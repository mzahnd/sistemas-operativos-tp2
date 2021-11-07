#ifndef AHORCADO
#define AHORCADO
#define AHORCADO_MAX_LENGTH 48
#include <stdlib.h>
#include <stdio.h>
#include <ahorcado.h>
#include <time.h>
#include <colors.h>
#include <stdGraphics.h>
static char *ahorcado_words[] = {"Hola", "Nariz", "Computadora", (char *)0};
static int ahorcado_total_words = 0;
static int ahorcado_choice = 0;
static int ahorcado_lives = 0;

static unsigned int ahorcado_guess[AHORCADO_MAX_LENGTH] = {0};

void init_ahorcado() {
  int total = 0;
  for (int i = 0; ahorcado_words[i] != 0; i++) {
    total++;
  }
  ahorcado_total_words = total;
  ahorcado_choice = (getSeconds() + getMinutes() * 60) % total;
  for (int i = 0; i < strlen(ahorcado_words[ahorcado_choice]); i++) {
    ahorcado_guess[i] = 0;
  }
  ahorcado_lives = 3;
}

void draw_word() {
  char str[strlen(ahorcado_words[ahorcado_choice])];
  int i;
  for (i = 0; i < strlen(ahorcado_words[ahorcado_choice]); i++) {
    if (ahorcado_guess[i] == 0) {
      str[i] = '_';
    }
    else {
      str[i] = ahorcado_words[ahorcado_choice][i];
    }
  }
  str[i] = 0;
  drawString(SCREEN_WIDTH/4 - round(strlen(str)/2.0 * 3 * BASE_CHAR_WIDTH), SCREEN_HEIGHT/2 + SCREEN_HEIGHT/4, str, strlen(str), BLACK, BLACK, 3, 1);
}

void draw_game_over() {
  drawString(SCREEN_WIDTH/4 - round(strlen("Game Over")/2.0 * 4 * BASE_CHAR_WIDTH), SCREEN_HEIGHT/2 + SCREEN_HEIGHT/4, "Game Over", strlen("Game Over"), RED, BLACK, 4, 1);
}
void draw_win() {
  drawString(SCREEN_WIDTH/4 - round(strlen("You Win")/2.0 * 4 * BASE_CHAR_WIDTH), SCREEN_HEIGHT/2 + SCREEN_HEIGHT/4 + BASE_CHAR_HEIGHT * 4, "You Win", strlen("You Win"), RED, BLACK, 4, 1);
}

void draw_background() {
  drawRect(0, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, PALE_PINK);
}

void draw_lives() {
  char lives[10] = {0};
  for (int i = 0; i < 10 && i < ahorcado_lives; i++) {
    lives[i] = 'X';
  }
  drawString(0, SCREEN_HEIGHT/2 + BASE_CHAR_HEIGHT*3, lives, strlen(lives), BLACK, BLACK, 3, 1);
}

int keyPressedAhorcado(char ch) {
  if (!isalpha(ch)) {
    return 0;
  }
  draw_background();
  int wrong = 1;
  for (int i = 0; i < strlen(ahorcado_words[ahorcado_choice]); i++) {
    if (ch == ahorcado_words[ahorcado_choice][i]) {
      ahorcado_guess[i] = 1;
      wrong = 0;
    }
  }
  int won = 1;
  for (int i = 0; i < strlen(ahorcado_words[ahorcado_choice]); i++) {
    if (ahorcado_guess[i] == 0) {
      won = 0;
    }
  }
  if (wrong) {
    ahorcado_lives--;
  }
  if (ahorcado_lives <= 0) {
    draw_game_over();
  } else {
    draw_lives();
    draw_word();
  }
  if (won) {
    draw_win();
  }
  char str[strlen(ahorcado_words[ahorcado_choice])];
  int i;
  for (i = 0; i < strlen(ahorcado_words[ahorcado_choice]); i++) {
    if (ahorcado_guess[i] == 0) {
      str[i] = '-';
    } else {
      str[i] = ahorcado_words[ahorcado_choice][i];
    }
  }
  str[i] = 0;
  return 1;
}


#endif

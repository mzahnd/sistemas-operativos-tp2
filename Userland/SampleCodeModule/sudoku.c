#ifndef SUDOKU
#define SUDOKU
#include <stdGraphics.h>
#include <colors.h>
#include <sudoku.h>
#include <stdio.h>
#include <stdlib.h>
static int board[9][9];
static int fixed[9][9];
static int currentX = 0;
static int currentY = 0;
static int sudokuTotal = 0;
static int sudokuWin = 0;
void init_values()
{
        board[0][0] = 3;
        board[0][2] = 5;
        board[0][3] = 4;
        board[0][5] = 2;
        board[0][7] = 6;
        board[1][0] = 4;
        board[1][1] = 9;
        board[1][3] = 7;
        board[1][4] = 6;
        board[1][6] = 1;
        board[1][8] = 8;
        board[2][0] = 6;
        board[2][3] = 1;
        board[2][5] = 3;
        board[2][6] = 2;
        board[2][7] = 4;
        board[2][8] = 5;
        board[3][2] = 3;
        board[3][3] = 9;
        board[3][6] = 5;
        board[3][7] = 8;
        board[4][0] = 9;
        board[4][1] = 6;
        board[4][4] = 5;
        board[4][5] = 8;
        board[4][6] = 7;
        board[4][8] = 3;
        board[5][1] = 8;
        board[5][2] = 1;
        board[5][3] = 3;
        board[5][5] = 4;
        board[5][7] = 9;
        board[5][8] = 2;
        board[6][1] = 5;
        board[6][3] = 6;
        board[6][5] = 1;
        board[6][6] = 4;
        board[7][0] = 2;
        board[7][3] = 5;
        board[7][4] = 4;
        board[7][5] = 9;
        board[7][7] = 7;
        board[8][0] = 1;
        board[8][1] = 4;
        board[8][2] = 9;
        board[8][5] = 7;
        board[8][6] = 3;
        board[8][8] = 6;
}

void init_fixed()
{
        fixed[0][0] = 1;
        fixed[0][2] = 1;
        fixed[0][3] = 1;
        fixed[0][5] = 1;
        fixed[0][7] = 1;
        fixed[1][0] = 1;
        fixed[1][1] = 1;
        fixed[1][3] = 1;
        fixed[1][4] = 1;
        fixed[1][6] = 1;
        fixed[1][8] = 1;
        fixed[2][0] = 1;
        fixed[2][3] = 1;
        fixed[2][5] = 1;
        fixed[2][6] = 1;
        fixed[2][7] = 1;
        fixed[2][8] = 1;
        fixed[3][2] = 1;
        fixed[3][3] = 1;
        fixed[3][6] = 1;
        fixed[3][7] = 1;
        fixed[4][0] = 1;
        fixed[4][1] = 1;
        fixed[4][4] = 1;
        fixed[4][5] = 1;
        fixed[4][6] = 1;
        fixed[4][8] = 1;
        fixed[5][1] = 1;
        fixed[5][2] = 1;
        fixed[5][3] = 1;
        fixed[5][5] = 1;
        fixed[5][7] = 1;
        fixed[5][8] = 1;
        fixed[6][1] = 1;
        fixed[6][3] = 1;
        fixed[6][5] = 1;
        fixed[6][6] = 1;
        fixed[7][0] = 1;
        fixed[7][3] = 1;
        fixed[7][4] = 1;
        fixed[7][5] = 1;
        fixed[7][7] = 1;
        fixed[8][0] = 1;
        fixed[8][1] = 1;
        fixed[8][2] = 1;
        fixed[8][5] = 1;
        fixed[8][6] = 1;
        fixed[8][8] = 1;
        sudokuTotal = 48;
}

int checkVertical(int i, int j);
int checkHorizontal(int i, int j);
int checkKernel(int i, int j);

void init_sudoku()
{
        for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                        board[i][j] = 0;
                        fixed[i][j] = 0;
                }
        }
        init_values();
        init_fixed();
}

void highlightCurrent(int startX, int startY, double scl)
{
        drawRect(round(startX + currentX * scl), round(startY + currentY * scl),
                 round(scl), round(scl), PALE_PINK);
}

void drawCells(int startX, int startY, double scl)
{
        int count = 0;
        for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                        int color;
                        if (i < 3 || i >= 6) {
                                if (j < 3 || j >= 6) {
                                        if (count % 2 == 0) {
                                                color = LIGHT_APPLE;
                                        } else {
                                                color = DARK_APPLE;
                                        }
                                } else {
                                        if (count % 2 == 0) {
                                                color = PALE_YELLOW;
                                        } else {
                                                color = DARK_PALE_YELLOW;
                                        }
                                }
                        } else {
                                if (j < 3 || j >= 6) {
                                        if (count % 2 == 0) {
                                                color = PALE_YELLOW;
                                        } else {
                                                color = DARK_PALE_YELLOW;
                                        }
                                } else {
                                        if (count % 2 == 0) {
                                                color = LIGHT_APPLE;
                                        } else {
                                                color = DARK_APPLE;
                                        }
                                }
                        }
                        drawRect(round(startX + j * scl),
                                 round(startY + i * scl), round(scl),
                                 round(scl), color);
                        count++;
                }
        }
}

void drawNumbers(int startX, int startY, double scl)
{
        sudokuWin = 1;
        for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                        int color = GRAY;
                        if (fixed[i][j] == 1) {
                                color = BLACK;
                        } else if (!(checkHorizontal(i, j) &&
                                     checkVertical(i, j) &&
                                     checkKernel(i, j))) {
                                color = RED;
                        }
                        if (board[i][j] != 0) {
                                char ch = board[i][j] + '0';
                                drawString(round(startX + j * scl + scl / 2 -
                                                 BASE_CHAR_WIDTH),
                                           round(startY + i * scl + scl / 2 -
                                                 BASE_CHAR_HEIGHT),
                                           &ch, 1, color, WHITE, 2, 1);
                        }
                }
        }
}

void draw_sudoku()
{
        drawRect(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2,
                 SCREEN_HEIGHT / 2, WHITE);
        int startX = SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4 - SCREEN_HEIGHT / 4;
        int startY = SCREEN_HEIGHT / 2;
        double scl = (SCREEN_HEIGHT / 2.0) / 9;
        drawCells(startX, startY, scl);
        highlightCurrent(startX, startY, scl);
        drawNumbers(startX, startY, scl);
        if (sudokuWin && sudokuTotal == 9 * 9) {
                drawString(SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4 -
                                   round(4 * BASE_CHAR_WIDTH * 3.5),
                           SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4 -
                                   2 * BASE_CHAR_HEIGHT,
                           "YOU WIN", strlen("YOU WIN"), RED, BLACK, 4, 0);
        }
}

int keyPressedSudoku(char key)
{
        if (key >= '0' && key <= '9' && fixed[currentY][currentX] != 1) {
                if (key == '0' && board[currentY][currentX] != 0) {
                        sudokuTotal--;
                } else if (key != '0' && board[currentY][currentX] == 0) {
                        sudokuTotal++;
                }
                board[currentY][currentX] = key - '0';
        } else if (key == (char)KEY_UP && currentY > 0) {
                currentY--;
        } else if (key == (char)KEY_DOWN && currentY < 8) {
                currentY++;
        } else if (key == (char)KEY_LEFT && currentX > 0) {
                currentX--;
        } else if (key == (char)KEY_RIGHT && currentX < 8) {
                currentX++;
        } else {
                return 0;
        }
        return 1;
}

int checkVertical(int i, int j)
{
        for (int ii = 0; ii < 9; ii++) {
                if (board[i][j] == board[ii][j] && ii != i) {
                        sudokuWin = 0;
                        return 0;
                }
        }
        return 1;
}

int checkHorizontal(int i, int j)
{
        for (int jj = 0; jj < 9; jj++) {
                if (board[i][j] == board[i][jj] && jj != j) {
                        sudokuWin = 0;
                        return 0;
                }
        }
        return 1;
}

int checkKernel(int i, int j)
{
        int kernelX = j / 3;
        int kernelY = i / 3;
        for (int ii = 0; ii < 3; ii++) {
                for (int jj = 0; jj < 3; jj++) {
                        int cordI = 3 * kernelY + ii;
                        int cordJ = 3 * kernelX + jj;
                        if (board[cordI][cordJ] == board[i][j] &&
                            (cordI != i || cordJ != j)) {
                                sudokuWin = 0;
                                return 0;
                        }
                }
        }
        return 1;
}

#endif

// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**
 * This file is part of sistemas-operativos-tp2
 * Licensed under BSD 3-Clause "New" or "Revised" License.
 * Copyright (c) 2022 Flores Levalle, M.
 *                    López, P.
 *                    Sierra Pérez, C.
 *                    Zahnd, M. E.
 */
#ifndef STD_GRAPHICS_H
#define STD_GRAPHICS_H

#include <stdint.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define BASE_CHAR_WIDTH 8
#define BASE_CHAR_HEIGHT 16

void drawString(int x, int y, char *buffer, int dim, uint64_t fontColor,
                uint64_t backgroundColor, int fontSize, int alphaBackground);
void clearScreen(uint64_t color);
void drawRect(int xi, int yi, int width, int height, uint64_t color);

#endif /* STD_GRAPHICS_H */

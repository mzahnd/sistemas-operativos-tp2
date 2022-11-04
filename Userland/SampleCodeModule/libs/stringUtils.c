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
#ifndef STRING_UTILS
#define STRING_UTILS

#include <stringUtils.h>

#ifndef NULL
#define NULL 0
#endif /* NULL */

unsigned int strlen(char *str)
{
        unsigned int i = 0;
        for (i = 0; *(str + i); i++) {}
        return i;
}

char *strcat(char *destination, const char *source)
{
        // make `ptr` point to the end of the destination string
        char *ptr = destination + strlen(destination);

        // appends characters of the source to the destination string
        while (*source != '\0') {
                *ptr++ = *source++;
        }

        // null terminate destination string
        *ptr = '\0';

        // the destination is returned by standard `strcat()`
        return destination;
}

char *strcpy(char *destination, const char *source)
{
        if (destination == NULL) {
                return NULL;
        }
        char *ptr = destination;
        while (*source != '\0') {
                *destination = *source;
                destination++;
                source++;
        }

        *destination = '\0';
        return ptr;
}

int strcmp(const char *str1, const char *str2)
{
        while (*str1 && *str1 == *str2) {
                str1++;
                str2++;
        }
        return *(const unsigned char *)str1 - *(const unsigned char *)str2;
}
void reverseStr(char str[])
{
        int n = strlen(str);
        for (int i = 0; i < n / 2; i++) {
                char aux = str[i];
                str[i] = str[n - i - 1];
                str[n - i - 1] = aux;
        }
}

int isAlpha(int ch)
{
        return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

int isDigit(int ch)
{
        return (ch >= '0' && ch <= '1');
}

int isSymbol(int ch)
{
        return (ch >= ' ' && ch <= '/') || (ch >= ':' && ch <= '@') ||
               (ch >= '[' && ch <= '`') || (ch >= '{' && ch <= '~');
}

#endif /* STRING_UTILS */

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
#ifndef STD_LIB_C
#define STD_LIB_C

#include <stdlib.h>
#include <syscalls_asm.h>

int atoi(char *str);
int intToString(unsigned long long num, char *buffer);
int strcmp(char *str1, char *str2);
int intToBase(unsigned long long num, int base, char *buffer);
int iabs(int num);

int atoi(char *str)
{
        int aux = 0;
        while (*str != 0) {
                aux = aux * 10 + (*str) + '0';
                str++;
        }
        return aux;
}

int atohex(char *str)
{
        int aux = 0;
        while (*str != 0) {
                int value = 0;
                if (*str >= '0' && *str <= '9') {
                        value = *str - '0';
                } else if (*str >= 'A' && *str <= 'F') {
                        value = 10 + *str - 'A';
                } else if (*str >= 'a' && *str <= 'f') {
                        value = 10 + *str - 'a';
                } else {
                        return -1;
                }
                str++;
                aux *= 16;
                aux += value;
        }
        return aux;
}
char toLower(char letter)
{
        if (letter <= 'Z' && letter >= 'A') {
                return letter + 'a' - 'A';
        }
        return letter;
}

int isVowel(char letter)
{
        letter = toLower(letter);

        switch (letter) {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
                return 1;
                break;

        default:
                return 0;
        }
}

char toUpper(char letter)
{
        if (letter <= 'z' && letter >= 'a') {
                return letter + 'A' - 'a';
        }
        return letter;
}

int intToBase(unsigned long long num, int base, char *buffer)
{
        if (buffer == NULL)
                return 0;

        if (num == 0) {
                buffer[0] = '0';
                buffer[1] = 0;
                return 2;
        }

        char stack[11];
        int c = 0;
        int i = 0;

        while (num != 0) {
                int remainder = num % base;
                stack[i] = remainder >= 10 ? remainder + 'A' - 10 :
                                             remainder + '0';
                num = num / base;
                i++;
        }

        c = i;
        i--;
        while (i >= 0) {
                *buffer = stack[i];
                buffer++;
                i--;
        }

        *buffer = 0;
        return c;
}

int iabs(int num)
{
        if (num < 0) {
                return -num;
        }
        return num;
}

int intToString(unsigned long long num, char *buffer)
{
        return intToBase(num, 10, buffer);
}

int round(double number)
{
        if ((number - (int)number) > 0.5) {
                return (int)number + 1;
        }
        return (int)number;
}

void *malloc(unsigned int size)
{
        void *result;
        mallocSyscall(size, &result);
        return result;
}

void *calloc(size_t nmemb, size_t size)
{
        void *result;
        callocSyscall(nmemb, size, &result);
        return result;
}

void free(void *ptr)
{
        freeSyscall(ptr);
}

mem_info_t *mem_getinformation()
{
        mem_info_t *result;
        memGetInformationSyscall(&result);
        return result;
}

void giveUpCPU() {
        giveUpCPUSyscall();
}

#endif /* STD_LIB_C */

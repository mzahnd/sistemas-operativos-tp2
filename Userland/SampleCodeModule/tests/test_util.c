// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**
 * This file is part of sistemas-operativos-tp2
 * Licensed under BSD 3-Clause "New" or "Revised" License.
 * Copyright (c) 2022 Flores Levalle, M.
 *                    López, P.
 *                    Sierra Pérez, C.
 *                    Zahnd, M. E.
 *
 * Test provistos por la cátedra.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
/* #include "syscall.h" */
#include <stdlib.h> /* NULL, atoi() */
#include <stdarg.h> /* Variadic */
#include <stringUtils.h> /* memcpy */
#include <tests/test_util.h>
#include <processManagement.h> /* getPid(); */

//Random
static uint32_t m_z = 362436069;
static uint32_t m_w = 521288629;

uint32_t GetUint()
{
        m_z = 36969 * (m_z & 65535) + (m_z >> 16);
        m_w = 18000 * (m_w & 65535) + (m_w >> 16);
        return (m_z << 16) + m_w;
}

uint32_t GetUniform(uint32_t max)
{
        uint32_t u = GetUint();
        return (u + 1.0) * 2.328306435454494e-10 * max;
}

//Memory
uint8_t memcheck(void *start, uint8_t value, uint32_t size)
{
        uint8_t *p = (uint8_t *)start;
        uint32_t i;

        for (i = 0; i < size; i++, p++)
                if (*p != value)
                        return 0;

        return 1;
}

//Parameters
int64_t satoi(char *str)
{
        uint64_t i = 0;
        int64_t res = 0;
        int8_t sign = 1;

        if (!str)
                return 0;

        if (str[i] == '-') {
                i++;
                sign = -1;
        }

        for (; str[i] != '\0'; ++i) {
                if (str[i] < '0' || str[i] > '9')
                        return 0;
                res = res * 10 + str[i] - '0';
        }

        return res * sign;
}

//Dummies
void bussy_wait(uint64_t n)
{
        uint64_t i;
        for (i = 0; i < n; i++)
                ;
}

int endless_loop(int argc, char **argv)
{
        while (1)
                ;
}

int endless_loop_print(int argc, char **argv)
{
        // unsigned int pid = getPid();
        // uint64_t wait = 0;
        // if (argc == 2)
        //         wait = atoi(argv[1]);
        if (argc != 2) {
                printf("Error: Endless loop needs an argument to print\n");
                return 1;
        }

        while (1) {
                printf("%s", argv[1]);
                bussy_wait(25000000);
        }
        return 0;
}

char **create_argv(char *argv0, int *argc, unsigned n, ...)
{
        va_list ap;

        char **argv = calloc(n + 2, sizeof(char));
        if (argv == NULL)
                return NULL;

        argv[0] = calloc(strlen(argv0), sizeof(char));
        if (argv[0] == NULL)
                return NULL;

        strcpy(argv[0], argv0);

        va_start(ap, n);
        int i;
        for (i = 1; i < n + 1; i++) {
                // uint64_t has 0xFFFFFFFFFFFFFFFF as maximum possible value:
                // that's 20 digits in base 10
                argv[i] = calloc(21, sizeof(char));
                if (argv[i] == NULL)
                        return NULL;

                intToString(va_arg(ap, unsigned long long), argv[i]);
        }
        argv[i] = NULL;
        va_end(ap);

        *argc = n + 2;
        return argv;
}

void free_argv(int argc, char **argv)
{
        for (int i = 0; i < argc; i++) {
                if (argv[i] != NULL)
                        free(argv[i]);
        }

        free(argv);
}

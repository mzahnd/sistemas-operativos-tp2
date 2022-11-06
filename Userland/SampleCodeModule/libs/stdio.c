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
#ifndef STD_IO
#define STD_IO

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <syscalls_asm.h>
#include <stdGraphics.h>
#include <stringUtils.h>

#define PRINTF_BUFFER_SIZE 128
#define STD_BUFFER_SIZE 256

#define INLINE_SPACING 16

char std_in[STD_BUFFER_SIZE] = { 0 };
char std_out[STD_BUFFER_SIZE] = { 0 };
static int updateConsoleInitialized = 0;

void (*updateConsolePointer)(char *, int);

ssize_t read(int fd, char *buf, size_t count)
{
        ssize_t ret;
        pipeReadSyscall(fd, buf, count, &ret);
        return ret;
}

ssize_t write(int fd, const char *buf, size_t count)
{
        int fds[2] = {-1, -1};
        getCurrentProcessFDSyscall(fds);
        if (fds[STDOUT] == STDOUT) {
                updateConsolePointer(buf, count);
                return count;
        }
        ssize_t ret;
        pipeWriteSyscall(fd, buf, count, &ret);
        return ret;
}

void scanf(char *buffer)
{
        int aux = 0;
        int index = 0;
        while (aux != '\n') {
                if (aux) {
                        buffer[index++] = aux;
                        putChar(aux);
                        aux = getChar();
                }
        }
        putChar('\n');
}

void printf(char *fmt, ...)
{
        va_list vl;
        va_start(vl, fmt);
        char *auxPtr;
        char buffer[128] = { 0 };
        char tmp[20];
        int i = 0, j = 0;
        while (fmt && fmt[i]) {
                if (fmt[i] == '%') {
                        i++;
                        switch (fmt[i]) {
                        case 'c':
                                buffer[j++] = (char)va_arg(vl, int);
                                break;
                        case 'd':
                                intToString(va_arg(vl, int), tmp);
                                strcpy(&buffer[j], tmp);
                                j += strlen(tmp);
                                break;
                        case 's':
                                auxPtr = va_arg(vl, char *);
                                strcpy(&buffer[j], auxPtr);
                                j += strlen(auxPtr);
                                break;
                        case 'x':
                                intToBase(va_arg(vl, int), 16, tmp);
                                strcpy(&buffer[j], tmp);
                                j += strlen(tmp);
                                break;
                        case 'X': //long hexa
                                intToBase(va_arg(vl, uint64_t), 16, tmp);
                                strcpy(&buffer[j], tmp);
                                j += strlen(tmp);
                                break;
                        }
                } else if (fmt[i] == '\t') {
                        while (j % INLINE_SPACING != 0) {
                                buffer[j++] = ' ';
                        }
                } else {
                        buffer[j++] = fmt[i];
                }
                i++;
        }
        

        va_end(vl);

        // Call to write syscall
        write(STDOUT, buffer, j);
}

void setConsoleUpdateFunction(void (*f)(char *, int))
{
        updateConsolePointer = f;
        updateConsoleInitialized = 1;
};

void putChar(char ch)
{
        updateConsolePointer(&ch, 1);
}

int getChar()
{
        int ch = 0;
        uint64_t count;
        pipeReadSyscall(STDIN, &ch, 1, &count);
        return ch;
}

void setFunctionKey(int index, void (*func)())
{
        setFunctionKeyMethodSyscall((uint64_t)index, func);
}

uint64_t getError()
{
        uint64_t err = 32;
        readErrorSyscall(&err);
        return err;
}

void format(char *str, int value)
{
        if (value < 10) {
                str[1] = value + '0';
        } else {
                str[0] = (value / 10) + '0';
                str[1] = (value % 10) + '0';
        }
        str[2] = 0;
}

#endif /* STD_IO */

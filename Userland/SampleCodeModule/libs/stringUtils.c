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

/* Type to use for aligned memory operations.
 *
 * Should normally be the biggest type supported by a single load and store.
 * Must be an unsigned type.
 *
 * Motivation: memset() glibc 2.36
 * https://sourceware.org/git/?p=glibc.git;a=blob;f=string/memcmp.c;h=40029474e6c9206e5c38711d60b011768f422f44;hb=refs/heads/release/2.36/master
 */
#define op_t unsigned long int
#define OPSIZ (sizeof(op_t))

unsigned int strlen(char *str)
{
        unsigned int i = 0;
        for (i = 0; *(str + i); i++) {}
        return i;
}

size_t strnlen(const char *s, size_t maxlen)
{
        if (s == NULL)
                return 0;

        size_t n = 0;

        while (n < maxlen && *s) {
                s++;
                n++;
        }

        return n;
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

char *strncpy(char *destination, const char *source, unsigned int num)
{
        if (destination == NULL) {
                return NULL;
        }
        char *ptr = destination;
        while (*source && num--) {
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
        return (ch >= '0' && ch <= '9');
}

int isSymbol(int ch)
{
        return (ch >= ' ' && ch <= '/') || (ch >= ':' && ch <= '@') ||
               (ch >= '[' && ch <= '`') || (ch >= '{' && ch <= '~');
}

int isWritable(int ch)
{
        return ch >= ' ' && ch <= '~';
}

void *memset(void *dest, int c, size_t len)
{
        /* Motivation: memset() from glibc 2.36
         *
         * https://sourceware.org/git/?p=glibc.git;a=blob;f=string/memset.c;h=1303dd7ad365f91d5fc07309aba573e2705a37b6;hb=refs/heads/release/2.36/master
        */
        op_t dest_op = (op_t)dest;

        if (len >= 8) {
                size_t chunk_len = 0;

                // Convert character 'c' to op_t type
                op_t c_op = (char)c;
                c_op |= c_op << 8;
                c_op |= c_op << 16;
                if (OPSIZ > 4)
                        // Two steps to avoid warning (false positive)
                        c_op |= (c_op << 16) << 16;

                // Align
                while (dest_op % OPSIZ != 0) {
                        ((char *)dest_op)[0] = c;

                        dest_op++;
                        len--;
                }

                // Big chunks first
                chunk_len = len / (OPSIZ * 8);
                while (chunk_len > 0) {
                        ((op_t *)dest_op)[0] = c_op;
                        ((op_t *)dest_op)[1] = c_op;
                        ((op_t *)dest_op)[2] = c_op;
                        ((op_t *)dest_op)[3] = c_op;
                        ((op_t *)dest_op)[4] = c_op;
                        ((op_t *)dest_op)[5] = c_op;
                        ((op_t *)dest_op)[6] = c_op;
                        ((op_t *)dest_op)[7] = c_op;

                        dest_op += 8 * OPSIZ;
                        chunk_len--;
                }
                len %= OPSIZ * 8;

                // Smaller (remaining) chunks
                // Write 'op_t' until less than OPSIZ bytes remain
                chunk_len = len / OPSIZ;
                while (chunk_len > 0) {
                        ((op_t *)dest_op)[0] = c_op;

                        dest_op += OPSIZ;
                        chunk_len--;
                }
                len %= OPSIZ;
        }

        // Bytes left
        while (len > 0) {
                ((char *)dest_op)[0] = c;

                dest_op++;
                len--;
        }

        return dest;
}

void *memcpy(void *dest, const void *src, size_t len)
{
        /* Motivation: memset() from glibc 2.36
         *
         * https://sourceware.org/git/?p=glibc.git;a=blob;f=string/memset.c;h=1303dd7ad365f91d5fc07309aba573e2705a37b6;hb=refs/heads/release/2.36/master
        */
        op_t dest_op = (op_t)dest;
        op_t src_op = (op_t)src;

        if (len >= 8) {
                size_t chunk_len = 0;

                // Align
                while (dest_op % OPSIZ != 0) {
                        ((char *)dest_op)[0] = ((char *)src_op)[0];

                        dest_op++;
                        src_op++;
                        len--;
                }

                // Big chunks first
                chunk_len = len / (OPSIZ * 8);
                while (chunk_len > 0) {
                        ((op_t *)dest_op)[0] = ((op_t *)src_op)[0];
                        ((op_t *)dest_op)[1] = ((op_t *)src_op)[1];
                        ((op_t *)dest_op)[2] = ((op_t *)src_op)[2];
                        ((op_t *)dest_op)[3] = ((op_t *)src_op)[3];
                        ((op_t *)dest_op)[4] = ((op_t *)src_op)[4];
                        ((op_t *)dest_op)[5] = ((op_t *)src_op)[5];
                        ((op_t *)dest_op)[6] = ((op_t *)src_op)[6];
                        ((op_t *)dest_op)[7] = ((op_t *)src_op)[7];

                        dest_op += 8 * OPSIZ;
                        src_op += 8 * OPSIZ;
                        chunk_len--;
                }
                len %= OPSIZ * 8;

                // Smaller (remaining) chunks
                // Write 'op_t' until less than OPSIZ bytes remain
                chunk_len = len / OPSIZ;
                while (chunk_len > 0) {
                        ((op_t *)dest_op)[0] = ((op_t *)src_op)[0];

                        dest_op += OPSIZ;
                        src_op += OPSIZ;
                        chunk_len--;
                }
                len %= OPSIZ;
        }

        // Bytes left
        while (len > 0) {
                ((char *)dest_op)[0] = ((char *)src_op)[0];

                dest_op++;
                src_op++;
                len--;
        }

        return dest;
}

#endif /* STRING_UTILS */

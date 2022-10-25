#include <stdio.h>
#include <commands.h>
#include <stdlib.h>
#include <stdGraphics.h>
#include <time.h>
#include <inforeg.h>
#include <shells.h>
#include <stdint.h>
#include <syscalls_asm.h>
#include <exceptions_asm.h>
#include <four_windows.h>

void dateTime(char args[MAX_ARGS][MAX_ARG_LEN])
{
        putChar('\n');
        char days[] = "00";
        char month[] = "00";
        char year[] = "00";
        char hours[] = "00";
        char minutes[] = "00";
        char seconds[] = "00";
        format(days, getDays());
        format(month, getMonth());
        format(year, getYear());
        format(hours, getHours());
        format(minutes, getMinutes());
        format(seconds, getSeconds());
        printf("%s/%s/20%s %s:%s:%s", days, month, year, hours, minutes,
               seconds);
}

void infoReg(char args[MAX_ARGS][MAX_ARG_LEN])
{
        uint64_t registers[19];
        getRegisters(registers);
        putChar('\n');
        printf("R15: %X - R14: %X\n", registers[18], registers[17]);
        printf("R13: %X - R12: %X\n", registers[16], registers[15]);
        printf("R11: %X - R10: %X\n", registers[14], registers[13]);
        printf("R9: %X - R8: %X\n", registers[12], registers[11]);
        printf("RSI: %X - RDI: %X\n", registers[10], registers[9]);
        printf("RBP: %X - RDX: %X\n", registers[8], registers[7]);
        printf("RCX: %X - RBX: %X\n", registers[6], registers[5]);
        printf("RAX: %X - RIP: %X\n", registers[4], registers[3]);
        printf("CS: %X - FLAGS: %X\n", registers[2], registers[1]);
        printf("RSP: %X\n", registers[0]);
}

void printmem(char args[MAX_ARGS][MAX_ARG_LEN])
{
        putChar('\n');
        int with0x = 0;
        if (args[1][0] == '0' && args[1][1] == 'x') {
                with0x = 2;
        }
        uint64_t aux = atohex(&args[1][with0x]);
        if (aux > 0) {
                uint64_t bytes[32];
                getMemSyscall(aux, bytes, 32);
                for (int i = 0; i < 4; i++) {
                        printf("0x%x: ", aux + i * 8);
                        for (int j = 0; j < 8; j++) {
                                printf("%x ", bytes[i * 8 + j]);
                        }
                        printf("\n");
                }
        }
}

void help(char args[MAX_ARGS][MAX_ARG_LEN])
{
        putChar('\n');
        printf("This is the Help Center\n");
        printf("\tSpecial keys:\n");
        printf("\t* F12 - saves the values of the registers\n");
        printf("\tCommands:\n");
        printf("\t* datetime - displays the current date and time of the OS\n");
        printf("\t* inforeg - displays the values of each register\n");
        printf("\t* printmem [ARGUMENT] - displays 32 bytes of memory,\n");
        printf("\tstarting from the address given in the argument\n");
        printf("\t* clear - clears the current shell\n");
        printf("\t* echo [ARGUMENT] - prints the given argument\n");
        printf("\t* divzero - forces a division by zero\n");
        printf("\t* invalidopcode - forces an invalid OP code\n");
        printf("\t* windows - open a four window application\n");
        printf("\t* twomallocs - Allocate two blocks of 1024 bytes each. \n");
        printf("\t* twofrees [ADDR1] [ADDR2] - Free blocks allocated by twomallocs \n");
}

void clear(char args[MAX_ARGS][MAX_ARG_LEN])
{
        clearAll();
}

void echo(char args[MAX_ARGS][MAX_ARG_LEN])
{
        putChar('\n');
        for (int i = 1; args[i][0] && i < MAX_ARGS; i++) {
                printf("%s ", args[i]);
        }
        putChar('\n');
}

void divzero(char args[MAX_ARGS][MAX_ARG_LEN])
{
        _divzero();
}

void invalidopcode(char args[MAX_ARGS][MAX_ARG_LEN])
{
        _invalidopcode();
}

void windows(char args[MAX_ARGS][MAX_ARG_LEN])
{
        init_apps();
        putChar('\n');
}

void twomallocs(char args[MAX_ARGS][MAX_ARG_LEN])
{
        void *result = 0;

        putChar('\n');
        mallocSyscall(1024, &result);
        ((char *)result)[0] = 'h';
        ((char *)result)[1] = 'o';
        ((char *)result)[2] = 'l';
        ((char *)result)[3] = 'a';
        printf("First malloc: %X\n ", result);

        mallocSyscall(1024, &result);
        ((char *)result)[0] = 'c';
        ((char *)result)[1] = 'h';
        ((char *)result)[2] = 'a';
        ((char *)result)[3] = 'u';
        printf("Second malloc: %X\n ", result);

        printf("Written 'hola' in first malloc; 'chau' in second\n", result);
}

void twofrees(char args[MAX_ARGS][MAX_ARG_LEN])
{
        void *addr1 = NULL;
        void *addr2 = NULL;
        if (args[1][0] == '0' && args[1][1] == 'x') {
                // Dual cast to avoid warning
                addr1 = (void *)(uint64_t)atohex(&args[1][2]);
        } else {
                addr1 = (void *)(uint64_t)atohex(&args[1][0]);
        }

        if (args[2][0] == '0' && args[2][1] == 'x') {
                // Dual cast to avoid warning
                addr2 = (void *)(uint64_t)atohex(&args[2][2]);
        } else {
                addr2 = (void *)(uint64_t)atohex(&args[2][0]);
        }

        putChar('\n');
        freeSyscall(addr1);
        printf("First malloc freed\n");
        freeSyscall(addr2);
        printf("Second malloc freed\n");
}

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
#ifndef BETTER_SHELL
#define BETTER_SHELL

#include <stdlib.h>
#include <stdio.h>
#include <stdGraphics.h>
#include <stringUtils.h>
#include <colors.h>
#include <BetterShell/betterShell.h>
#include <BetterShell/shellLines.h>
#include <BetterShell/commandList.h>
#include <processManagement.h>
#include <processes.h>
#include <pipeUser.h>

#define SHELL_BG_COLOR 0x001017 //BUTTERFLY_BUSH
#define DETACH_PROCESS_CHAR '&'
#define MAX_COMMAND_TOKENS 128

typedef int (*processFunciton)(int, char *);

static int writeToCommandLine(int ch, char *commandLine, unsigned int *index,
                              shellLinesQueue lines);
static void displayCommandLine(char *commandLine, unsigned int index);
static void deleteLastChar(char *commandLine, unsigned int *index);
static void undrawLastChar(char *commandLine, unsigned int index);
static void drawLastChar(char *commandLine, unsigned int index);
static void clearCommandLine(char *commandLine, unsigned int *index);
static void processCommand(char *command, commandList commands,
                           unsigned int *indexPtr);
static void addArgToArgv(char **argv, unsigned int index, char *str,
                         unsigned int strDim);
static processFunciton getProcess(char *name);
static void addToTokens(char *tokens[MAX_COMMAND_TOKENS], char *token, unsigned int *index);
static void executeCommand(commandList commands, char ** argv, int argc, unsigned int stdin, unsigned int stdout);

void setupArgv(char **argv, int argc, char *command, unsigned int commandLen);
void printOnShell(char *str, int dim);
static void initCommands(commandList list);


static shellLinesQueue lines;

int testPrint1(int argc, char** argv) {
        printf("ASD");
}

int testRead1(int argc, char** argv) {
        char* buffer[64] = {0};
        read(STDIN, buffer, 64);
        printf("Read from STDIN: [%s]\n", buffer);
}

int testProcess2(int argc, char **argv)
{
        for (int i = 0; i < 50; i++) {
                printf("Proceso 2: %d\n", i);
        }

        return 0;
}

int testProcess3(int argc, char **argv)
{
        for (int i = 0; i < 50; i++) {
                printf("ASDASDSADASDA: %d\n", i);
        }

        return 0;
}

int runShell(int argc, char **argv)
{
        unsigned int commandLineIndex = 0;
        char *commandLine = malloc((MAX_COMMAND_LENGTH + 1) * sizeof(char));
        commandList commands = newCommandList();
        initCommands(commands);
        // addCommand(commands, "test", testPipes);
        // addCommand(commands, "testRead", testRead);
        // addCommand(commands, "testWrite", testWrite);

        lines = newShellLines(64);

        setConsoleUpdateFunction(printOnShell);
        printf("Welcome to our Operating Systems proyect!\n");
        printf("For further options, enter 'help'");

        if (commandLine == NULL) {
                //TODO: Exit
                return -1;
        }
        for (int i = 0; i <= MAX_COMMAND_LENGTH; i++) {
                commandLine[i] = '\0';
        }

        displayCommandLine(commandLine, commandLineIndex);
        displayLines(lines);
        // Game Loop
        while (1) {
                int inputChar = getChar();
                // Write the char
                if (writeToCommandLine(inputChar, commandLine,
                                       &commandLineIndex, lines)) {
                        processCommand(commandLine, commands,
                                       &commandLineIndex);
                }
        }

        free(commandLine);

        return 0;
}

static int writeToCommandLine(int ch, char *commandLine, unsigned int *index,
                              shellLinesQueue lines)
{
        //Writes a char to the last avaiable position of the commandLine
        if (ch == '\b') {
                deleteLastChar(commandLine, index);
                undrawLastChar(commandLine, *index);
        } else if (ch == '\n') {
                commandLine[*index] = ch;
                (*index)++;
                addShellLine(lines, commandLine);
                displayLines(lines);
                //Process Command
                return 1;

        } else if (*index < (MAX_COMMAND_LENGTH - 1) &&
                   (isAlpha(ch) || isDigit(ch) || isSymbol(ch))) {
                commandLine[*index] = ch;
                (*index)++;
                drawLastChar(commandLine, *index - 1);
        }
        return 0;
}

static void displayCommandLine(char *commandLine, unsigned int index)
{
        drawRect(0, SCREEN_HEIGHT - BASE_CHAR_HEIGHT, SCREEN_WIDTH,
                 BASE_CHAR_HEIGHT, SHELL_BG_COLOR);
        drawString(0, SCREEN_HEIGHT - BASE_CHAR_HEIGHT, commandLine, index,
                   WHITE, SHELL_BG_COLOR, 1, 0);
}

static void deleteLastChar(char *commandLine, unsigned int *index)
{
        if (*index > 0) {
                commandLine[*index] = '\0';
                (*index)--;
        }
}

static void drawLastChar(char *commandLine, unsigned int index)
{
        drawString(index * BASE_CHAR_WIDTH, SCREEN_HEIGHT - BASE_CHAR_HEIGHT,
                   &commandLine[index], 1, WHITE, SHELL_BG_COLOR, 1, 0);
}

static void undrawLastChar(char *commandLine, unsigned int index)
{
        drawRect(index * BASE_CHAR_WIDTH, SCREEN_HEIGHT - BASE_CHAR_HEIGHT,
                 BASE_CHAR_WIDTH, BASE_CHAR_HEIGHT, SHELL_BG_COLOR);
}

static void clearCommandLine(char *commandLine, unsigned int *index)
{
        for (int i = 0; i < *index; i++) {
                commandLine[i] = 0;
        }
        *index = 0;
}

static void processCommand(char *command, commandList commands,
                           unsigned int *lenPtr)
{
        if (command == NULL) {
                //print ERROR
                return;
        }
        unsigned int commandLen = *lenPtr;
        if (commandLen == 0) {
                //print ERROR
                return;
        }

        char* tokens[MAX_COMMAND_TOKENS];
        unsigned int totalTokens = 0;

        char token[MAX_COMMAND_LENGTH] = {0};
        unsigned int tokenIndex = 0;
        for (int i = 0; i < commandLen; i++) {
                if (command[i] == ' ') {
                        if (tokenIndex == 0) {
                                continue;
                        }
                        addToTokens(tokens, token, &totalTokens);
                        //push to argv
                        for (int j = 0; j < tokenIndex; j++) {
                                token[j] = '\0';
                        }
                        tokenIndex = 0;
                        continue;
                }
                token[tokenIndex] = command[i];
                tokenIndex++; 
        }
        token[tokenIndex-1] = '\0'; //To remove \n
        addToTokens(tokens, token, &totalTokens);

        int totalCommands = 0;
        unsigned int argc = 0;
        char ** argv = malloc(sizeof(char *));

        int pipes[MAX_COMMAND_TOKENS][2] = {0}; 
        int pipeIndex = 0;

        for (int i = 0; i < totalTokens; i++) {
                if (strcmp(tokens[i], "|") == 0) {
                        
                        pipe(pipes[pipeIndex]);
                        if (pipeIndex == 0) { // If it is the first command 
                                executeCommand(commands, argv, argc, 0, pipes[0][PIPE_FD_WRITE]);
                        } else {
                                executeCommand(commands, argv, argc, pipes[pipeIndex-1][PIPE_FD_READ], pipes[pipeIndex][PIPE_FD_WRITE]);
                        }
                        for (int j = 0; j < argc; j++) {
                                argv[j] = NULL;
                        }
                        argc = 0;
                        pipeIndex++;
                        totalCommands++;
                        continue;
                }
                argv[argc] = tokens[i];
                argc++;
        }

        if (pipeIndex > 0) {
                executeCommand(commands, argv, argc, pipes[pipeIndex-1][PIPE_FD_READ], 1);
        } else {
                executeCommand(commands, argv, argc, 0, 1);
        }

        free(argv);
        for (int i = 0; i < totalTokens; i++) {
                free(tokens[i]);
        }


        clearCommandLine(command, lenPtr);
        displayCommandLine(command, *lenPtr);
}

void printOnShell(char *str, int dim)
{
        if (lines == NULL) {
                return;
        }
        char buffer[MAX_COMMAND_LENGTH] = { 0 };
        for (int i = 0, k = 0; i < dim && i < MAX_COMMAND_LENGTH; i++, k++) {
                buffer[k] = str[i];
                if (str[i] == '\n' || i == (dim - 1) ||
                    i == (MAX_COMMAND_LENGTH - 1)) {
                        char lastLineChar =
                                lines->last ?
                                        lines->last
                                                ->line[strlen(lines->last->line) -
                                                       1] :
                                        'x';
                        if (!lines->last || lastLineChar == '\n') {
                                addShellLine(lines, buffer);
                        } else {
                                addToLastLine(lines, buffer);
                        }
                        for (int j = 0; j <= i; j++) {
                                buffer[j] = 0;
                        }
                        k = -1;
                }
        }
        addToLastLine(lines, buffer);
        displayLines(lines);
}

void setupArgv(char **argv, int argc, char *command, unsigned int commandLen)
{
        if (command == NULL)
                return;

        char buffer[MAX_COMMAND_LENGTH] = { 0 };
        unsigned int argIndex = 0;

        for (int i = 0, j = 0; i < commandLen; i++, j++) {
                if (command[i] == ' ') {
                        if (command[i + 1] && command[i + 1] != ' ') {
                                addArgToArgv(argv, argIndex, buffer, j);
                                argIndex++;
                        }
                        for (int k = 0; k <= j; k++) {
                                buffer[k] = 0;
                        }
                        j = -1;
                } else if (command[i + 1] == '\0') {
                        addArgToArgv(argv, argIndex, buffer, j);
                        argIndex++;
                } else {
                        buffer[j] = command[i];
                }
        }
}

int testProcessShell(int argc, char **argv)
{
        if (argc > 0) {
                for (int i = 0; i < argc; i++) {
                        printf("[%d]: %s\n", i, argv[i]);
                }
        }
        return 0;
}

static void addArgToArgv(char **argv, unsigned int index, char *str,
                         unsigned int strDim)
{
        if (argv == NULL || str == NULL) {
                return;
        }

        argv[index] = malloc(strDim * sizeof(char) + 1);
        if (argv[index] == NULL) {
                return;
        }

        strcpy(argv[index], str);
        argv[index][strDim + 1] = '\0';
}

static void addToTokens(char *tokens[MAX_COMMAND_TOKENS], char *token, unsigned int *index) {
        tokens[*index] = malloc(*index * sizeof(char));
        strcpy(tokens[*index], token);
        (*index)++;
}

static void executeCommand(commandList commands, char ** argv, int argc, unsigned int stdin, unsigned int stdout) {
        unsigned int type = 0;
        processMainFunction_t function = getCommand(commands, argv[0], &type);
        unsigned int foreground = 1;
        if (argc > 1 && strcmp(argv[argc-1], "&") == 0) {
                argv[argc-1] = NULL;
                argc--;
                foreground = 0;
        }

        if (function == NULL) {
                printf("[%s] Is not a valid command\n", argv[0]);
                return;
        }
        uint64_t pid = createProcessWithFD(argv[0], function, argc, argv, foreground, stdin, stdout);
        if (foreground) {
                waitPID(pid);
        }
}
static void initCommands(commandList list) {
        if(list == NULL) {
                return;
        }
        addCommand(list, "test", testPipes);
        addCommand(list, "testRead", testRead);
        addCommand(list, "testWrite", testWrite);
//         addCommand(list, "sh", commandSh);
        addCommand(list, "help", commandHelp);
        addCommand(list, "mem", commandMem);
        addCommand(list, "ps", commandPs);
//         addCommand(list, "loop", commandLoop);
//         addCommand(list, "kill", commandKill);
        addCommand(list, "nice", commandNice);
//         addCommand(list, "block", commandBlock);
        addCommand(list, "sem", commandSem);
//         addCommand(list, "cat", commandCat);
//         addCommand(list, "wc", commandWc);
//         addCommand(list, "filter", commandFilter);
        addCommand(list, "pipe", commandPipe);
//         addCommand(list, "phylo", commandPhylo);
}

#endif /* BETTER_SHELL */

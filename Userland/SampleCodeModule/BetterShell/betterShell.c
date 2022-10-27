#ifndef BETTER_SHELL
#define BETTER_SHELL

#include <stdlib.h>
#include <stdio.h>
#include <stdGraphics.h>
#include <colors.h>
#include <BetterShell/betterShell.h>
#include <BetterShell/shellLines.h>

#define SHELL_BG_COLOR 0x001017 //BUTTERFLY_BUSH

static void writeToCommandLine(char ch, char *commandLine, unsigned int* index, shellLinesQueue lines);
static void displayCommandLine(char * commandLine, unsigned int index);
static void deleteLastChar(char* commandLine, int* index);
static void undrawLastChar(char* commandLine, int index);
static void drawLastChar(char* commandLine, int index);
static void clearCommandLine(char* commandLine, unsigned int* index);

int runShell(int argc, char** argv) {
    
    unsigned int commandLineIndex = 0;
    char * commandLine = malloc((MAX_COMMAND_LENGTH + 1) * sizeof(char));

    shellLinesQueue lines = newShellLines(5);
    addShellLine(lines, "Hola");
    addShellLine(lines, "ASDASDDAS");
    addShellLine(lines, "Esto es un test xD");


    if (commandLine == NULL) {
        //TODO: Exit
        return;
    }
    for (int i = 0; i <= MAX_COMMAND_LENGTH; i++) {
        commandLine[i] = '\0';
    }


    displayCommandLine(commandLine, commandLineIndex);
    displayLines(lines);
    // Game Loop
    while(1) {
        char inputChar = getChar();
        // Write the char
        writeToCommandLine(inputChar, commandLine, &commandLineIndex, lines);
    }

    free(commandLine);
}

static void writeToCommandLine(char ch, char* commandLine, unsigned int* index, shellLinesQueue lines) {
    //Writes a char to the last avaiable position of the commandLine
    if (ch == '\b') {
        deleteLastChar(commandLine, index);
        undrawLastChar(commandLine, *index);
    } else if (ch == '\n') {
        addShellLine(lines, commandLine);
        displayLines(lines);
        clearCommandLine(commandLine, index);
        displayCommandLine(commandLine, *index);
    } else if (*index < (MAX_COMMAND_LENGTH - 1) && ch >= 0) {
        commandLine[*index] = ch;
        (*index)++;
        drawLastChar(commandLine, *index-1);
    }
}

static void displayCommandLine(char* commandLine, unsigned int index) {
    drawRect(0, SCREEN_HEIGHT - BASE_CHAR_HEIGHT, SCREEN_WIDTH, BASE_CHAR_HEIGHT, SHELL_BG_COLOR);
    drawString(0, SCREEN_HEIGHT - BASE_CHAR_HEIGHT, commandLine, index, WHITE, SHELL_BG_COLOR, 1, 0);
}

static void deleteLastChar(char* commandLine, int* index) {
    if (*index > 0) {
        commandLine[*index] = '\0';
        (*index)--;
    }
}

static void drawLastChar(char* commandLine, int index) {
    drawString(index * BASE_CHAR_WIDTH, SCREEN_HEIGHT - BASE_CHAR_HEIGHT, &commandLine[index], 1, WHITE, SHELL_BG_COLOR, 1, 0);
}

static void undrawLastChar(char* commandLine, int index) {
    drawRect(index * BASE_CHAR_WIDTH, SCREEN_HEIGHT - BASE_CHAR_HEIGHT, BASE_CHAR_WIDTH, BASE_CHAR_HEIGHT, SHELL_BG_COLOR);
}

static void clearCommandLine(char* commandLine, unsigned int* index) {
    for (int i = 0; i < *index; i++) {
        commandLine[i] = 0;
    }
    *index = 0;

}




#endif
#ifndef BETTER_SHELL
#define BETTER_SHELL

#include <stdlib.h>
#include <stdio.h>
#include <stdGraphics.h>
#include <stringUtils.h>
#include <colors.h>
#include <BetterShell/betterShell.h>
#include <BetterShell/shellLines.h>
#include <processManagement.h>

#define SHELL_BG_COLOR 0x001017 //BUTTERFLY_BUSH
#define DETACH_PROCESS_CHAR '&'

typedef int (*processFunciton)(int,char*);

static void writeToCommandLine(char ch, char *commandLine, unsigned int* index, shellLinesQueue lines);
static void displayCommandLine(char * commandLine, unsigned int index);
static void deleteLastChar(char* commandLine, unsigned int* index);
static void undrawLastChar(char* commandLine, unsigned int index);
static void drawLastChar(char* commandLine, unsigned int index);
static void clearCommandLine(char* commandLine, unsigned int* index);
static void processCommand(char* command);
static void addArgToArgv(char ** argv, unsigned int index, char * str, unsigned int strDim);
static processFunciton getProcess(char* name);
void setupArgv(char** argv, int argc, char *command, unsigned int commandLen);
void printOnShell(char* str, int dim);

static shellLinesQueue lines;

int runShell(int argc, char** argv) {
    
    unsigned int commandLineIndex = 0;
    char * commandLine = malloc((MAX_COMMAND_LENGTH + 1) * sizeof(char));

    lines = newShellLines(64);

    setConsoleUpdateFunction(printOnShell);


    if (commandLine == NULL) {
        //TODO: Exit
        return -1;
    }
    for (int i = 0; i <= MAX_COMMAND_LENGTH; i++) {
        commandLine[i] = '\0';
    }

    // printf("Hola\n");
    // printf("Otra Linea\n");
    printf("Line 1\nLine 2 ");
    printf("ASDASD = ");
    printf("%X.\n", 0x11234);


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
        commandLine[*index] = ch;
        (*index)++;
        addShellLine(lines, commandLine);
        displayLines(lines);

        //Process Command
        processCommand(commandLine);

        clearCommandLine(commandLine, index);
        displayCommandLine(commandLine, *index);


    } else if (*index < (MAX_COMMAND_LENGTH - 1) && (isAlpha(ch) || isDigit(ch) || isSymbol(ch))) {
        commandLine[*index] = ch;
        (*index)++;
        drawLastChar(commandLine, *index-1);
    }
}

static void displayCommandLine(char* commandLine, unsigned int index) {
    drawRect(0, SCREEN_HEIGHT - BASE_CHAR_HEIGHT, SCREEN_WIDTH, BASE_CHAR_HEIGHT, SHELL_BG_COLOR);
    drawString(0, SCREEN_HEIGHT - BASE_CHAR_HEIGHT, commandLine, index, WHITE, SHELL_BG_COLOR, 1, 0);
}

static void deleteLastChar(char* commandLine, unsigned int* index) {
    if (*index > 0) {
        commandLine[*index] = '\0';
        (*index)--;
    }
}

static void drawLastChar(char* commandLine, unsigned int index) {
    drawString(index * BASE_CHAR_WIDTH, SCREEN_HEIGHT - BASE_CHAR_HEIGHT, &commandLine[index], 1, WHITE, SHELL_BG_COLOR, 1, 0);
}

static void undrawLastChar(char* commandLine, unsigned int index) {
    drawRect(index * BASE_CHAR_WIDTH, SCREEN_HEIGHT - BASE_CHAR_HEIGHT, BASE_CHAR_WIDTH, BASE_CHAR_HEIGHT, SHELL_BG_COLOR);
}

static void clearCommandLine(char* commandLine, unsigned int* index) {
    for (int i = 0; i < *index; i++) {
        commandLine[i] = 0;
    }
    *index = 0;

}

static void processCommand(char* command) {
    if (command == NULL) {
        //print ERROR
        return;
    }
    int commandLen = strlen(command);
    if (commandLen == 0) {
        //print ERROR
        return;
    }

    unsigned int foreground = 1; // Process is foreground by default
    unsigned int argc = 1; // At least 1

    for (int i = 0; i < commandLen; i++) {
	if (command[i] == DETACH_PROCESS_CHAR && i > 0 && command[i] == ' ') {
		foreground = 0;
		argc--;
	}
        if (command[i] == ' ' && command[i+1] && command[i+1] != '\n' && command[i+1] != DETACH_PROCESS_CHAR && command[i+1] != ' ') { 
		argc++; 
	} 
    }

    char** argv = malloc(argc * sizeof(char*));

    setupArgv(argv, argc, command, commandLen);

    // Here I have argc and argv
    createProcess(argv[0], (int(*)(int,char**))(getProcess(argv[0])), argc, argv);

    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    } 
    free(argv); 
}


void printOnShell(char* str, int dim) {
    if (lines == NULL) {
        return;
    }
    char buffer[MAX_COMMAND_LENGTH] = {0};
    for (int i = 0, k = 0; i < dim && i < MAX_COMMAND_LENGTH; i++, k++) {

        buffer[k] = str[i];
        if (str[i] == '\n' || i == (dim-1) || i == (MAX_COMMAND_LENGTH-1)) {
            char lastLineChar = lines->last ? lines->last->line[strlen(lines->last->line)-1] : 'x';
            if (!lines->last || lastLineChar == '\n') {
                addShellLine(lines, buffer);
            } else {
                addToLastLine(lines, buffer);
            }
            for (int j = 0; j <= i; j++) {
                buffer[j] = 0;
            }
            k=-1;
        }
    }
    addToLastLine(lines, buffer);
    displayLines(lines);
}

void setupArgv(char** argv, int argc, char *command, unsigned int commandLen) {
    char buffer[MAX_COMMAND_LENGTH] = {0};
    unsigned int argIndex = 0;
    for (int i = 0, j = 0; i < commandLen; i++, j++) {
        if (command[i] == ' ') {
            if (command[i+1] && command[i+1] != ' ') {
                addArgToArgv(argv, argIndex, buffer, j);
                argIndex++;
            }
            for (int k = 0; k <= j; k++) {
                buffer[k] = 0;
            }
            j = -1;
        } else if (command[i+1] == '\0') {
            addArgToArgv(argv, argIndex, buffer, j);
            argIndex++;
        } else {
            buffer[j] = command[i];
        }     
    }

}

int testProcessShell(int argc, char** argv) {
    if (argc > 0) {
        for (int i = 0; i < argc; i++) {
            printf("[%d]: %s\n", i, argv[i]);
        }
    }
    return 0;
}

static processFunciton getProcess(char* name) {
    return (processFunciton)testProcessShell;
}

static void addArgToArgv(char ** argv, unsigned int index, char * str, unsigned int strDim) {
    if (argv == NULL || str == NULL) {
        return;
    }
    argv[index] = malloc(strDim * sizeof(char) + 1);
    strcpy(argv[index], str);
    argv[index][strDim+1] = '\0';
}




#endif

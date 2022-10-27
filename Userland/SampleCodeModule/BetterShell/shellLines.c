#ifndef SHELL_LINES
#define SHELL_LINES

#include <BetterShell/shellLines.h>
#include <stdlib.h>
#include <stdGraphics.h>
#include <colors.h>

static void freeNextNodeRec(shellLineNode node);
static void freeNode(shellLineNode node);

shellLinesQueue newShellLines(unsigned int maxLines) {
    shellLinesQueue lines = malloc(sizeof(shellLinesQueue_t));
    if (lines != NULL) {
        lines->first = NULL;
        lines->last = NULL;
        lines->maxLines = maxLines;
        lines->size = 0;
    } 
    return lines;
}

void freeShellLines(shellLinesQueue lines) {
    if (lines == NULL) {
        return;
    }
    if (lines->first != NULL) {
        freeNextNodeRec(lines->first);
    }
    free(lines);
}

void addShellLine(shellLinesQueue lines, char * str) {
    unsigned int len = strlen(str);
    char * line = malloc((len + 1) * sizeof(char));
    strcpy(line, str);

    shellLineNode node = malloc(sizeof(shellLineNode_t));
    if (node == NULL) {
        //TODO: Exit
        return;
    }
    node->line = line;
    node->next = NULL;

    if (lines->size == 0) {
        lines->first = node;
        lines->last = node;
        lines->size++;
        return;
    }

    lines->last->next = node;
    lines->last = node;
    lines->size++;

    if (lines->size == lines->maxLines) {
        shellLineNode firstToDelete = lines->first;
        lines->first = lines->first->next;
        freeNode(firstToDelete);
        lines->size--;
    }
}


void displayLines(shellLinesQueue lines) {
    // Draw Background
    drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - BASE_CHAR_HEIGHT, BUTTERFLY_BUSH);
    
    if (lines == NULL || lines->size == 0) {
        return;
    }

    shellLineNode node = lines->first;
    for (int i = 0; i < lines->maxLines && i < lines->size; i++) {
        drawString(0, SCREEN_HEIGHT - BASE_CHAR_HEIGHT * (lines->size-i+1), node->line, strlen(node->line), WHITE, BUTTERFLY_BUSH, 1, 0);
        node = node->next;
    }
    
}

static void freeNextNodeRec(shellLineNode node) {
    if (node == NULL) {
        return;
    }
    if (node->next != NULL) {
        freeNextNodeRec(node->next);
    }
    freeNode(node);
    return;
}

static void freeNode(shellLineNode node) {
    if (node == NULL) {
        return;
    }
    free(node->line);
    free(node);
    return;
}




#endif
#ifndef SHELL_LINES_H
#define SHELL_LINES_H

typedef struct shellLineNode_t {
    struct shellLineNode_t * next;
    char * line;
} shellLineNode_t;

typedef shellLineNode_t* shellLineNode;

typedef struct shellLinesQueue_t {
    shellLineNode first;
    shellLineNode last;
    unsigned int maxLines;
    unsigned int size;
} shellLinesQueue_t;

typedef shellLinesQueue_t* shellLinesQueue;

shellLinesQueue newShellLines(unsigned int maxLines);
void freeShellLines(shellLinesQueue lines);
void addShellLine(shellLinesQueue lines, char * str);
void displayLines(shellLinesQueue lines);


#endif
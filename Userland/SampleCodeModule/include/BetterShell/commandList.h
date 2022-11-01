#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

#define PROCESS_COMMAND 1
#define FUNCTION_COMMAND 2

typedef struct commandNode_t {
        struct commandNode_t *next;
        char *name;
        int (*command)(int, char **);
        int type;
};

#endif
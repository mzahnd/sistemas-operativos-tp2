#ifndef COMMAND_LIST_H
#define COMMAND_LIST_H

#define PROCESS_COMMAND 1
#define FUNCTION_COMMAND 2


typedef int (*processMainFunction_t)(int, char **);

typedef struct commandNode_t {
        struct commandNode_t *next;
        char *name;
        processMainFunction_t command;
        int type;
} commandNode_t;

typedef struct commandList_t {
        commandNode_t *firstCommand;
        commandNode_t *currentCommand;
	unsigned int size;
} commandList_t;

typedef commandNode_t * commandNode;
typedef commandList_t * commandList;   

commandList newCommandList();
void addCommand(commandList commandLinkedList, char *name, processMainFunction_t funtion);
processMainFunction_t getCommand(commandList list, char* name, unsigned int* type);

void initProcesses(commandList list);
#endif

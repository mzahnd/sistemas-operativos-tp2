#include <BetterShell/commandList.h>
#include <stdlib.h>
#include <stringUtils.h>

commandNode_t *addCommandRec(commandNode_t *aux, char *name, processMainFunction_t function, unsigned int* sizePtr);

commandList newCommandList()
{
	commandList list = malloc(sizeof(commandList_t));
	list->firstCommand = NULL;
	list->currentCommand = NULL;
	list->size = 0;
        return list;
}

void addCommand(commandList commandLinkedList, char *name,
                processMainFunction_t function)
{
	if (commandLinkedList == NULL) {
		return;
	}
        commandLinkedList->firstCommand =
                addCommandRec(commandLinkedList->firstCommand, name, function, &(commandLinkedList->size));
}

commandNode_t *addCommandRec(commandNode_t *aux, char *name,
                             processMainFunction_t function, unsigned int* sizePtr)
{
        if (aux == NULL) {
                commandNode_t *newCommand = malloc(sizeof(commandNode_t));
                newCommand->name = name;
                newCommand->next = NULL;
                newCommand->command = function;
		(*sizePtr)++; // Increase the size by 1
                return newCommand;
        }
        aux->next = addCommandRec(aux->next, name, function, sizePtr);
        return aux;
}

processMainFunction_t getCommand(commandList list, char* name, unsigned int* type) {
	if (list == NULL || list->size == 0) {
		return NULL;
	}
	commandNode node = list->firstCommand;
        while (node && strcmp(name, node->name) != 0) {
                node = node->next;
        }
        if (!node) {
                return NULL;
        }
        *type = node->type;
        return node->command;
}

int testA(int argc, char** argv) {
        printf("He vivido\n");
        return 0;
}

void initProcesses(commandList list) {
        if (list == NULL) {
                return;
        }
        addCommand(list, "test", testA);
}

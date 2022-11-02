#include "../include/BetterShell/commandList.h"
#include "../include/stdlib.h"
// #include "commandList.h"

typedef struct commandList {
        commandNode_t *firstCommand;
        commandNode_t *currentCommand;
} commandList;

typedef processMainFunction_t int (*)(int, char **);

commandList newCommandList()
{
        return calloc(1, sizeof(commandList));
}

void addCommand(commandList commandLinkedList, char *name,
                processMainFunction_t funtion)
{
        commandLinkedList->firstCommand =
                addCommandRec(commandLinkedList->firstCommand, name, fuction);
}

commandNode_t *addCommandRec(commandNode_t *aux, char *name,
                             processMainFunction_t funtion)
{
        if (firstCommand == NULL) {
                commandNode_t *newCommand = malloc(sizeof(commandNode_t));
                newCommand->name = name;
                newCommand->next = aux;
                newCommand->command = function;
                return newCommand;
        }
        aux->next = addCommandRec(aux->tail, name, function);
        return aux;
}

//Un get al que le pasas el nombre y te devuelve el puntero
//a la funcion, y que por parametro de salida te devuelva el tipo de comando.
//Si no esta el comando, retorna NULL.

void getFunction(commandList commandLinkedList, char *name, int type)
{
        commandLinkedList->firstCommand = getFunctionRec(name, type);
}

processMainFunction_t *getFunctionRec(char *name, int type)
{
        if (currentCommand == NULL) {
                return NULL;
        }

        if (currentCommand.name == name) {
                type = currentCommand.type;
                return currentCommand.(*command)(int, char **);
        }
        currentCommand->next = getFunctionRec(name, &type);
}

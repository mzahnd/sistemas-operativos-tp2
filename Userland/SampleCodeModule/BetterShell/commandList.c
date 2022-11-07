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
#include <BetterShell/commandList.h>
#include <stdlib.h>
#include <stringUtils.h>

commandNode_t *addCommandRec(commandNode_t *aux, char *name,
                             processMainFunction_t function,
                             unsigned int *sizePtr);

commandList newCommandList()
{
        commandList list = malloc(sizeof(commandList_t));
        if (list == NULL)
                return NULL;

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
                addCommandRec(commandLinkedList->firstCommand, name, function,
                              &(commandLinkedList->size));
}

commandNode_t *addCommandRec(commandNode_t *aux, char *name,
                             processMainFunction_t function,
                             unsigned int *sizePtr)
{
        if (aux == NULL) {
                commandNode_t *newCommand = malloc(sizeof(commandNode_t));
                if (newCommand == NULL)
                        return NULL;

                newCommand->name = name;
                newCommand->next = NULL;
                newCommand->command = function;
                (*sizePtr)++; // Increase the size by 1
                return newCommand;
        }
        aux->next = addCommandRec(aux->next, name, function, sizePtr);
        return aux;
}

processMainFunction_t getCommand(commandList list, char *name,
                                 unsigned int *type)
{
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

#ifndef PROCESS_RSC
#define PROCESS_RSC

#include <BetterShell/processRscList.h>
#include <stdlib.h>
#include <stdio.h>
#include <pipeUser.h>
#include <processManagement.h>

static void freeArgv(int argc, char **argv);
static void closeFDs(int stdin, int stdout);

rscList newRscList()
{
        rscList list = malloc(sizeof(rscList_t));
        list->size = 0;
        list->first = NULL;
        list->size = NULL;
        return list;
}

int addResources(rscList list, unsigned int pid, int argc, char **argv,
                 int stdin, int stdout)
{
        if (list == NULL) {
                return -1;
        }
        rscNode node = malloc(sizeof(rscNode_t));
        node->pid = pid;
        node->argc = argc;
        node->argv = argv;
        node->stdin = stdin;
        node->stdout = stdout;
        node->next = NULL;
        if (list->size == 0) {
                list->first = node;
        } else {
                list->last->next = node;
        }
        list->last = node;
        list->size++;
        return 1;
}

int removeResources(rscList list, int pid)
{
        if (list == NULL || list->size == 0) {
                return -1;
        }
        rscNode node = list->first;
        rscNode prev = NULL;
        while (node != NULL && node->pid != pid) {
                prev = node;
                node = node->next;
        }
        if (node == NULL) { // Not found
                return -1;
        }

        if (prev == NULL) { //first Node
                list->first = node->next; // NULL if size of list is 1
        } else {
                prev->next = node->next;
        }

        if (node == list->last) {
                list->last = prev;
        }
        list->size--;
        free(node);
        return 1;
}

void checkAndFreeRsc(rscList list)
{
        if (list == NULL || list->size == 0) {
                return;
        }

        rscNode node = list->first;
        rscNode prev = NULL;
        while (node != NULL) {
                if (!isProcessActive(node->pid)) {
                        rscNode next = node->next;
                        freeArgv(node->argc, node->argv);
                        //closeFDs(node->stdin, node->stdout);
                        if (node == list->first) {
                                list->first = next;
                        } else {
                                prev->next = next;
                        }
                        if (node == list->last) {
                                list->last = next; // next is NULL
                        }
                        printf("freeing resources of process %d\n", node->pid);
                        free(node);
                        list->size--;
                        node = next;
                        continue;
                }
                prev = node;
                node = node->next;
        }
}

static void freeArgv(int argc, char **argv)
{
        for (int i = 0; i < argc; i++) {
                free(argv[i]);
        }
        free(argv);
}

static void closeFDs(int stdin, int stdout)
{
        if (stdin != STDIN) {
                close(stdin);
        }

        if (stdout != STDOUT) {
                close(stdout);
        }
}

#endif
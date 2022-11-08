#ifndef PROCESS_RSC_H
#define PROCESS_RSC_H

typedef struct rscNode_t {
        struct rscNode_t *next;
        unsigned int pid;
        int argc;
        char **argv;
        int stdin;
        int stdout;
} rscNode_t;

typedef rscNode_t *rscNode;

typedef struct rscList_t {
        rscNode first;
        rscNode last;
        int size;
} rscList_t;

typedef rscList_t *rscList;

rscList newRscList();
int addResources(rscList list, unsigned int pid, int argc, char **argv,
                 int stdin, int stdout);
void checkAndFreeRsc(rscList list);

#endif
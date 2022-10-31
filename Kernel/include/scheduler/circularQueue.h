#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H
#include <scheduler/process.h>

typedef struct node_t {
        struct node_t *next;
        process pcb;
} node_t;

typedef node_t *node;

typedef struct circularQueue_t {
        node first;
        node last;
        int size;
} circularQueue_t;

typedef circularQueue_t *circularQueue;

circularQueue newCircularQueue();
int addToQueue(circularQueue queue, process p);
int removeFromQueue(circularQueue queue, uint64_t pid);
char *getFromQueue(circularQueue queue, int index);
process getFromPID(circularQueue queue, uint64_t pid);
#endif

#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H
#include <scheduler/process.h>

typedef struct node_t {
    struct node_t * next;
    process pcb;
    int space_filler[64]; //TODO Erase Space Filler
} node_t;

typedef node_t * node;

typedef struct circularQueue_t{
    node first;
    node last;
    int size;
    int space_filler[64]; //TODO Erase Space FillerS
} circularQueue_t;

typedef circularQueue_t * circularQueue;

circularQueue newCircularQueue();
int addToQueue(circularQueue queue, process p);
int removeFromQueue(circularQueue queue, uint64_t pid);
char * getFromQueue(circularQueue queue, int index);

#endif
#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

typedef struct node_t {
    struct node_t * next;
    char * name;
    int status;
} node_t;

typedef node_t * node;

typedef struct circularQueue_t{
    node first;
    node last;
    int size;
} circularQueue_t;

typedef circularQueue_t * circularQueue;

circularQueue newCircularQueue();
int addToQueue(circularQueue queue, char * name);
int removeFromQueue(circularQueue queue, char * name);
char * getFromQueue(circularQueue queue, int index);

#endif
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

#endif /* CIRCULAR_QUEUE_H */

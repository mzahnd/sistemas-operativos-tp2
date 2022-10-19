// #ifndef CIRCULAR_QUEUE
// #define CIRCULAR_QUEUE

// #include <scheduler/circularQueue.h>
// #include <memory.h>

// int deleteNode(circularQueue queue, node prev, node current, node next);

// circularQueue newCircularQueue() {
//     circularQueue queue = somalloc(sizeof(circularQueue_t));
//     queue->first = NULL;
//     queue->last = NULL;
//     queue->size = 0;
//     return queue;
// }

// int addToQueue(circularQueue queue, char * name) {
//     if (queue == NULL || name == NULL) {
//         return -1;
//     }
//     node n = somalloc(sizeof(node_t));
//     char * nameCopy = somalloc(sizeof(char) * strlen(name));
//     strcpy(nameCopy, name);
//     n->name = nameCopy;
//     n->status = 1;
    
//     if (queue->size == 0) {
//         queue->first = n;
//         queue->last = n;
//         n->next = n;
//     } else {
//         node lastNode = queue->last;
//         lastNode->next = n;
//         n->next = queue->first;
//         queue->last = n;
//     }
//     queue->size++;
//     return 1;
// }

// int removeFromQueue(circularQueue queue, char * name) {
//     if (queue == NULL || queue->size == 0) {
//         return -1;
//     }
//     node current = queue->first;
//     node prev = queue->last;
//     int finished = 0;
//     while (!finished) {
//         if (strcmp(current->name, name) == 0) {
//             deleteNode(queue, prev, current, current->next); //devuelve 1 si se elimino
//             return 1;
//         }
//         prev = current;
//         current = current->next;
//         if (current == queue->first) { // Si ya di una vuelta
//             finished = 1;
//         }
//     }
//     return -1;
// }

// int deleteNode(circularQueue queue, node prev, node current, node next) {
//     if (prev == current && current == next) {
//         if (queue->size != 1) {
//             exit(1);
//         }

//         sofree(current->name);
//         sofree(current);

//         queue->first = NULL;
//         queue->last = NULL;
//         queue->size = 0;
//         return 1;
//     }
//     if (queue->first == current) {
//         queue->first = next;
//     }
//     if (queue->last == current) {
//         queue->last = prev;
//     }
//     queue->size--;

//     prev->next = next;
//     sofree(current->name);
//     sofree(current);
//     return 1;
// }
// #endif
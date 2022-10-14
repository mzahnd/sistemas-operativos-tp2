#ifndef DUMMY_MEM
#define DUMMY_MEM

#define SIZE_FOR_NODES (0x100000)
#define TOTAL_NODES ((SIZE_FOR_NODES/sizeof(mnode)) + 1)
#define MEM_INIT 0x600000
#define MEM_END  0xFFFFFFF
#define HEAP_ADDRESS (MEM_INIT + sizeof(mnode) * TOTAL_NODES)
#define TOTAL_SIZE (MEM_END - MEM_INIT)

#include <stdint.h>
#include <dummy_mem.h>

typedef struct mnode
{
    uint32_t size;
    void * address;
    struct mnode * next;
} mnode;

typedef struct m_list {
    mnode * head;
    mnode * last;
} m_list;

typedef struct f_list
{
    mnode * head;
} f_list;

static mnode * node_array = (mnode*)MEM_INIT;
static int node_index = 0;
static int mem_init = 0;
static int free_init = 0;
static uint64_t free_mem = TOTAL_SIZE;
static m_list memList;
static f_list freeList;

static mnode * getNextNode() {
    if (node_index == TOTAL_NODES) return NULL;
    return &node_array[node_index++];
}

static void * init_mem(uint64_t size_in_bytes) {
    mnode * toAddNode = getNextNode();
    if (toAddNode == NULL) {
        return NULL;
    }
    toAddNode->size = size_in_bytes;
    toAddNode->address = (void*)HEAP_ADDRESS;
    toAddNode->next = NULL;

    memList.head = toAddNode;
    memList.last = toAddNode;
    mem_init = 1;
    return toAddNode->address;
}

void createMM(void * const base_address) {
    //Pass
}


void * somalloc(const uint64_t size_in_bytes) {
    if (size_in_bytes <= 0) {
        return NULL;
    }

    if (free_init) {
        mnode * current = NULL;
        mnode * prev = NULL;

        current = freeList.head;
        while(current->size < size_in_bytes && current->next != NULL) {
            prev = current;
            current = current->next;
        }
        if (current->size >= size_in_bytes) { //Si llegue al final, me fijo si entra en el ultimo
            if (freeList.head == current) { //Si encontre en el primero tengo que hacer que el primero sea el siguiente
                if (current->next == NULL) { //solo habia uno
                    freeList.head = NULL;
                    free_init = 0;
                } else {
                    freeList.head = current->next;
                }
            } 
        }
        if (prev != NULL) {
            prev->next = current->next;
        }
        return current->address;
    }

    //Si no tengo nodos libres, agrego uno al final de la lista
    if (!mem_init) { //Si la lista esta vacia, creo el primero
        if (init_mem(size_in_bytes) == NULL) {
            return NULL;
        }
        return memList.head->address;
    }

    mnode * toAddNode = getNextNode();
    if (toAddNode == NULL) {
        return NULL;
    }
    toAddNode->size = size_in_bytes;
    toAddNode->next = NULL;
    void * adddress = (void *)(memList.last->address + memList.last->size);
    toAddNode->address = adddress;
    memList.last->next = toAddNode;
    memList.last = toAddNode;
    return adddress;
}

void sofree(void * address) {
    if (!mem_init) return;

    mnode * current = memList.head;
    mnode * prev = NULL;

    while(current->address < address && current != NULL) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) { // No es una direccion valida
        return;
    }

    mnode * toAddFree = getNextNode();
    if (toAddFree == NULL) {
        return; //TODO 
    }
    toAddFree->address = current->address;
    toAddFree->size = current->size;


    if (!free_init) {
        freeList.head = toAddFree;
        toAddFree->next = NULL;
        free_init = 1;
        return;
    } else {
        current = freeList.head;
        prev = NULL;
        while (current->size < toAddFree->size && current != NULL) {
            prev = current;
            current = current->next;
        }
        if (current == freeList.head) { // Si es el primero
            freeList.head = current;
        }
        toAddFree->next = current;
    }

}

void mem_state(){
    // if(!mem_initialized){
    //     printS("No asignaste memoria dinamica!\n");
    // }
    // else{
    //     Node * iterator = mem_list.first;
    //     while(iterator->next != NULL){        
    //         for(uint8_t i = 0; i < iterator->size; i++){
    //             printS("0x");
    //             printHex( (uint64_t) iterator->address+i);
    //             printS(": ");
    //             printHex( memContent((uint8_t *)iterator->address+i) );
    //             newline();
    //         }
    //         iterator = iterator->next;
    //     }
    // }
}

#endif
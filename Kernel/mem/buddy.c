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
#ifdef BUDDY
#pragma message("Memory manager: Buddy")

#include <lib.h>
#include <mem/memory.h>

/* ------------------------------ */

#ifdef TESTING
#pragma message("TESTING defined")
#undef MEM_HEAP_START_ADDR
#define MEM_HEAP_START_ADDR (test_get_mem_heap_start_addr())
#endif /* TESTING */

/* ------------------------------ */

/* Masks and alignment */
#define BYTE_ALIGNMENT 32
#define BYTE_ALIGNMENT_MASK (0x001f)

/* Block related macros and definitions */
#define MEMBLOCK_SIZE                                      \
        ((sizeof(node_t) + (size_t)(BYTE_ALIGNMENT - 1)) & \
         ~((size_t)BYTE_ALIGNMENT_MASK))

#define MINIMUM_BLOCK_SIZE ((size_t)((MEMBLOCK_SIZE) << 1))

/* Tree */
#define BUDDY_TREE_START \
        ((void *)(MEM_HEAP_START_ADDR + MEM_HEAP_SIZE)) // At heap's end
#define BUDDY_TREE_LEVELS (get_level(MINIMUM_BLOCK_SIZE))
#define BUDDY_TREE_SIZE \
        (sizeof(node_t) * POW2(BUDDY_TREE_LEVELS + 1)) // Keep as POW2

#define TREE_LEFT_CHILD(index) (2 * (index) + 1)
#define TREE_RIGHT_CHILD(index) (2 * (index) + 2)
#define TREE_PARENT(index) (((index)-1) / 2)

#define POW2(x) (1 << (x))

/* ------------------------------ */
enum { AVAILABLE = 0, IN_USE = 1 };

typedef struct NODE {
        int state;
        unsigned int index;
        /* unsigned int level; */

        void *address;
} node_t;

/* ------------------------------ */

/* Prototypes */
static void mem_init();
static unsigned int get_level(size_t size);
static node_t *get_leftmost_node(unsigned int level);

#ifdef TESTING
inline static uint64_t test_get_mem_heap_start_addr();
#endif /* TESTING */

/* ------------------------------ */

/* Globals */
static node_t *tree_root = NULL;

/* ------------------------------ */

void *somalloc(size_t size)
{
        if (size == 0)
                return NULL;

        if (tree_root == NULL)
                mem_init();

        // Everything is already allocated
        if (tree_root[0].address != NULL)
                return NULL;

        unsigned int level = get_level(size);
        node_t *node = get_leftmost_node(level);
        if (node == NULL)
                return NULL;

        /* The memory map is stored as a binary tree, so all properties hold
         * true, specially how many leafs (blocks) are in a tree with a given
         * height (level) and the number of nodes in a perfect full binary 
         * tree.
         * 
         * This can tell us, first the number of blocks in the node's 
         * level (NB), and second, what's the index of the leftmost node in the
         * three at our level (FIDX).
         * Having this information, we "divide" the heap in NB blocks, and
         * take the memory address of the node index relative to FIDX.
         *
         * See: https://en.wikipedia.org/wiki/Binary_tree#Properties_of_binary_trees
         */
        unsigned int n_blocks_level = POW2(level);
        unsigned int first_index_in_level = POW2(level) - 1;

        /* This is the operator[] equation that C uses for arrays under the 
         * hood.
         * In an array with elements of size
         * (MEM_HEAP_START_ADDR / n_blocks_level), that starts at 
         * MEM_HEA_START_ADDR, this line would be:
         * node->address = (void*) arr[node->index - first_index_in_level];
         */
        node->address = (void *)(MEM_HEAP_START_ADDR +
                                 (MEM_HEAP_START_ADDR / n_blocks_level) *
                                         (node->index - first_index_in_level));

        return node->address;
}

/* ------------------------------ */

void *socalloc(size_t nmemb, size_t size)
{
        if (nmemb == 0 || size == 0)
                return NULL;

        void *ptr = somalloc(size * nmemb);
        if (ptr == NULL)
                return NULL;

        somemset(ptr, 0, size * nmemb);

        return ptr;
}

/* ------------------------------ */

void sofree(void *ptr)
{
        if (ptr == NULL)
                return;
        // Prevent user trying to free memory out of heap boundaries
        else if (ptr < (void *)MEM_HEAP_START_ADDR || //-V566
                 ptr > (void *)(MEM_HEAP_START_ADDR + MEM_HEAP_SIZE)) //-V566
                return;

        unsigned int index = 0;
        while (tree_root[index].address != ptr) {
                index++;

                if (index >= BUDDY_TREE_SIZE)
                        return;
        }

        tree_root[index].address = NULL;
        tree_root[index].state = AVAILABLE;

        // "Join" blocks
        unsigned int parent = 0;
        unsigned int brother = 0;

        if (index > 0)
                parent = TREE_PARENT(index);
        else
                parent = 0;

        while (parent > 0) {
                if (index % 2 == 0)
                        brother = TREE_LEFT_CHILD(parent);
                else
                        brother = TREE_RIGHT_CHILD(parent);

                if (tree_root[brother].state == AVAILABLE) {
                        tree_root[parent].state = AVAILABLE;
                        tree_root[parent].address = NULL;

                        index = parent;
                        parent = TREE_PARENT(index);
                } else {
                        parent = 0;
                }
        }

        // Root node
        if (tree_root[TREE_LEFT_CHILD(0)].state == AVAILABLE &&
            tree_root[TREE_RIGHT_CHILD(0)].state == AVAILABLE) {
                tree_root[0].state = AVAILABLE;
                tree_root[0].address = NULL;
        }
}

/* ------------------------------ */

somem_info_t *somem_getinformation()
{
        return NULL;
}

/* ------------------------------ */

static void mem_init()
{
        size_t buddy_tree_size = BUDDY_TREE_SIZE;

        somemset(BUDDY_TREE_START, 0, BUDDY_TREE_SIZE);
        BUDDY_TREE_LEVELS; // Calculate max level

        tree_root = (node_t *)BUDDY_TREE_START;
        tree_root->address = NULL;
        tree_root->index = 0;
        tree_root->state = AVAILABLE;
}

/* ------------------------------ */

static unsigned int get_level(size_t size)
{
        // Calculate max_level only once
        // Every call to BUDDY_TREE_LEVELS requests this value
        static unsigned int max_level = 0;

        unsigned int level = 0;
        unsigned int dividend = 1;

        if (size < MINIMUM_BLOCK_SIZE)
                size = MINIMUM_BLOCK_SIZE;

        if (size == MINIMUM_BLOCK_SIZE && max_level > 0)
                return max_level;

        while (MEM_HEAP_SIZE / dividend > size) {
                level++;
                dividend <<= 1;
        }

        if (size == MINIMUM_BLOCK_SIZE)
                max_level = level;

        return level;
}

/* ------------------------------ */

static node_t *get_leftmost_node(unsigned int level)
{
        if (level > BUDDY_TREE_LEVELS)
                return NULL;

        unsigned int parent = 0;
        unsigned int n_blocks_level = POW2(level);
        unsigned int first_index_in_level = n_blocks_level - 1; // 2^(level) - 1

        unsigned int index = first_index_in_level;
        int found = 0;

        while (found == 0) {
                while (tree_root[index].state != AVAILABLE) {
                        index++;

                        if (index >= first_index_in_level + n_blocks_level)
                                return NULL;
                }

                if (index % 2 == 0) {
                        found = 1;
                } else {
                        // Assume it's a valid node
                        found = 1;
                        // Check that no parent has been used
                        parent = TREE_PARENT(index);
                        while (parent > 0) {
                                if (tree_root[parent].address != NULL) {
                                        // Brother will have the same problem
                                        index += 2;
                                        found = 0;
                                        break;
                                }

                                parent = TREE_PARENT(parent);
                        }
                }
        }

        tree_root[index].index = index;
        tree_root[index].state = IN_USE;

        // Mark parents as "IN_USE"
        parent = 0;
        if (index > 0)
                parent = TREE_PARENT(index);

        tree_root[0].state = IN_USE;
        while (parent != 0) {
                tree_root[parent].state = IN_USE;
                tree_root[parent].address = NULL;

                parent = TREE_PARENT(parent);
        }

        return &tree_root[index];
        /*
        unsigned int current_level = 0;
        // How many times to the right we've descended
        unsigned int rdepth = 0;
        // How many times have we hit tree_root node
        int hit_root = 0;

        while (hit_root < 2) {
                while (current_level < level) {
                        index = TREE_LEFT_CHILD(index);
                        current_level++;
                }

                if (tree_root[index].state == AVAILABLE) {
                        tree_root[index].index = index;
                        tree_root[index].state = IN_USE;

                        // Mark parents as "IN_USE"
                        unsigned int parent = TREE_PARENT(index);
                        tree_root[0].state = IN_USE;
                        while (parent != 0) {
                                tree_root[parent].state = IN_USE;
                                parent = TREE_PARENT(parent);
                        }

                        return &tree_root[index];
                }

                if (index % 2 == 0) { // Right child
                        while (rdepth > 0) {
                                index = TREE_PARENT(index);
                                rdepth--;
                                current_level--;
                        }

                        index = TREE_PARENT(index);
                        if (index == 0)
                                hit_root++;

                        index = TREE_RIGHT_CHILD(index);

                        rdepth = 1;
                } else { // Left child
                        index = TREE_PARENT(index);
                        if (index == 0)
                                hit_root++;

                        index = TREE_RIGHT_CHILD(index);
                        rdepth++;
                }
        }

        // Hit root == 2 --> no blocks available
        return NULL;
        */
}

/* ------------------------------ */

#ifdef TESTING
inline static uint64_t test_get_mem_heap_start_addr()
{
        static uint64_t heap_mem_addr[MEM_HEAP_SIZE + (POW2(22 + 1))] = { 0 };

        return (uint64_t)&heap_mem_addr;
}
#endif /* TESTING */

#endif /* BUDDY */

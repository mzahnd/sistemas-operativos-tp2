#ifndef DUMMY_MEM_H
#define DUMMY_MEM_H

#define NULL 0
#include <stdint.h>

void createMM(void * const base_address);
void * somalloc(const uint64_t size_in_bytes);
void sofree(void * mem_address);
void mem_state();

#endif
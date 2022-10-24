#ifndef TEST_PROCESS
#define TEST_PROCESS
#include <processes.h>
#include <stdio.h>


int testProcess(int argc, char** argv) {
    for (int i = 0; i < 100; i++) {
        printf("HOLA %d\n", i);
    }
}

#endif
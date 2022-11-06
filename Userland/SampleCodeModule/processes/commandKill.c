#include <stdio.h> /* printf() */
#include <stdlib.h> /* NULL */

int commandKill(int argc, char **argv) {
    printf('\n');
    unsigned int id = atoi(argv[1]);
    if(kill(id) == 0) 
        printf("Process successfully killed\n");
    else
        printf("Process kill failed\n");
    
    return 1;
}


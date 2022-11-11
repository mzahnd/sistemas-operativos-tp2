#include <processes.h>
#include <stdio.h>

int commandHelp(int argc, char **argv)
{
        printf("This is the Help Center\n");
        printf("\tCommands:\n");
        printf("\t mem - displays the memory status\n");
        printf("\t ps - displays list of processes with their properties\n");
        printf("\t loop - prints current process ID\n");
        printf("\t activeLoop - prints current process ID but doesn't block the process while waiting\n");
        printf("\t kill [PID] - kills process with given ID\n");
        printf("\t nice [PID,priority] - switches process priority with given ID and priority\n");
        printf("\t block [PID] - switches process state to blocked with given ID\n");
        printf("\t sem - displays list of semaphores with their properties\n");
        printf("\t cat - displays stdin\n");
        printf("\t wc - prints amount of input lines\n");
        printf("\t filter - deletes vowels from the input\n");
        printf("\t pipe - displays list of pipes with their properties\n");
        printf("\t phylo - starts phylo app\n");
        printf("\t test_mm - memory manager test\n");
        printf("\t test_prio - priority test \n");
        printf("\t test_processes - processes test \n");
        printf("\t test_sync - sync test\n");
        return 0;
}

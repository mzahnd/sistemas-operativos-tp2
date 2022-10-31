#ifndef PROCESS_MANAGEMENT_H
#define PROCESS_MANAGEMENT_H

void createProcess(char * name, int (*mainF)(int, char**), int argc, char ** argv, unsigned int foreground);

#endif
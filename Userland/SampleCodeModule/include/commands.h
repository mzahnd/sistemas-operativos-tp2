#ifndef COMMANDS_H
#define COMMANDS_H
#define MAX_ARGS 10
#define MAX_ARG_LEN 32
#define MAX_LINES 20
#define BUFFER_SIZE 1024

void dateTime(char args[MAX_ARGS][MAX_ARG_LEN]);
void infoReg(char args[MAX_ARGS][MAX_ARG_LEN]);
void help(char args[MAX_ARGS][MAX_ARG_LEN]);
void clear(char args[MAX_ARGS][MAX_ARG_LEN]);
void echo(char args[MAX_ARGS][MAX_ARG_LEN]);
void printmem(char args[MAX_ARGS][MAX_ARG_LEN]);
void divzero(char args[MAX_ARGS][MAX_ARG_LEN]);
void invalidopcode(char args[MAX_ARGS][MAX_ARG_LEN]);
void windows(char args[MAX_ARGS][MAX_ARG_LEN]);

void sh(char args[MAX_ARGS][MAX_ARG_LEN]);

int mem(char args[MAX_ARGS][MAX_ARG_LEN]);

int ps(char args[MAX_ARGS][MAX_ARG_LEN]);
int loopFunc(char args[MAX_ARGS][MAX_ARG_LEN]);
int killFunc(char args[MAX_ARGS][MAX_ARG_LEN]);
int niceFunc(char args[MAX_ARGS][MAX_ARG_LEN]);
int blockFunc(char args[MAX_ARGS][MAX_ARG_LEN]);
int unblockFunc(char args[MAX_ARGS][MAX_ARG_LEN]);

int sem(char args[MAX_ARGS][MAX_ARG_LEN]);

int catFunc(char args[MAX_ARGS][MAX_ARG_LEN]);
int wcFunc(char args[MAX_ARGS][MAX_ARG_LEN]);
int filter(char args[MAX_ARGS][MAX_ARG_LEN]);
int pipe(char args[MAX_ARGS][MAX_ARG_LEN]);
int phylo(char args[MAX_ARGS][MAX_ARG_LEN]);


#endif

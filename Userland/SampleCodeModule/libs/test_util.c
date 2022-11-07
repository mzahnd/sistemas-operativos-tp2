#include <stdint.h>

int GetUint();
int GetUniform(int max);
int memcheck(void *start, int value, int size);
void *memoryset(void *b, int c, int len);
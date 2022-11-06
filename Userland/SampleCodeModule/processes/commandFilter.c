#include <processes.h>

int commandFilter(int argc, char **argv) {
    char c;
    printf('\n');

    char buffer[50];
    unsigned int i = 0;

    while((c = getChar()) != '\n'){
        putChar(c);
        if(!isVowel(c)) {
            // strcat(buffer, c, &i);
            buffer[i++] = c;
        }
    }
    buffer[i] = 0;
    printf('\n');
    printf("Filtered word: %s\n", buffer);
    processKiller();

    return 1;
}
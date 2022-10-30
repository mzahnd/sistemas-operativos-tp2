#ifndef STRING_UTILS_H
#define STRING_UTILS_H

unsigned int strlen(char *);
char* strcat(char* destination, const char* source);
char* strcpy(char* destination, const char* source);
void reverseStr(char str[]);
int isAlpha(char ch);
int isDigit(char ch);
int isSymbol(char ch);
#endif
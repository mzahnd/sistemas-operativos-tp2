#ifndef STDLIB_H
#define STDLIB_H

int atoi(char *str);
int intToString(unsigned long long num, char *buffer);
int strlen(char *str);
int strcmp(char *str1, char *str2);
int intToBase(unsigned long long num, int base, char *buffer);
int iabs(int num);
char *strcpy(char *dest, char *src);
int atohex(char *str);
char toUpper(char letter);
char toLower(char letter);
int round(double number);
int isalpha(char ch);
void reverseStr(char str[]);
void* malloc(unsigned int size);
void free(void *ptr);

#endif

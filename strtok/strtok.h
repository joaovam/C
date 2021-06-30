#ifndef STRTOK_H
#define STRTOK_H

typedef struct splitted
{
    char* str;
    struct splitted *next;

}splitted;

char* test(char*,char*);
splitted* split(char*, char*);

#endif
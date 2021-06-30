#include "strtok.h"
#include <string.h>
#include <stdlib.h>

char* test(char* string,char* pattern ){
    char* pointer = strstr(string,pattern);

    if(pointer!=NULL){
        *pointer = '\0';
        pointer+=strlen(pattern);
    }

    return pointer;
}

splitted* split(char* string, char* pattern){
    int len = strlen(string);
    
    char *clone = (char*) malloc(sizeof(char) *strlen(string) );
    strcpy(clone,string);
    splitted *result = (splitted*)malloc(sizeof(splitted));
    splitted *walker = result;

    walker->str = string;
    walker->next = malloc(sizeof(splitted));
    walker = walker->next;
    char *tail = test(string,pattern);

    while(tail!=NULL && tail < &string[len]){
        walker->str = tail;
        walker->next = malloc(sizeof(splitted));
        walker = walker->next;
        tail = test(tail,pattern);
    }

    walker = NULL;

    free(clone);
    return result;
}




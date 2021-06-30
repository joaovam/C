#include <stdio.h>
#include "strtok.h"

#include <stdio.h>

int main(int argc, char** argv){

    char test[] ="testing oo message oo finally";
    splitted *result = split(test,"oo");
    splitted *walker = result;
    
    while(walker->next!=NULL){
        printf("result = %s\n",walker->str);
        walker = walker->next;
    }
}
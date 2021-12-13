#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "Person_p.h"

person_t* person_new(){
    return malloc(sizeof(person_t));
}
void person_ctor(person_t* person, const char* fName, const char* Lname){
    strcpy(person->first_name,fName);
    strcpy(person->last_name,Lname);
    
}
void person_dtor(person_t* person){
    //Nothing
}

void person_get_first_name(person_t* person,char* fName){
    strcpy(fName,person->first_name);
}
void person_get_last_name(person_t* person,char* lName){
    strcpy(lName,person->last_name);
}
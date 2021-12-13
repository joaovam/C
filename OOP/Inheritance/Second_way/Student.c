#include <string.h>
#include <stdlib.h>
#include "Person.h"
#include "Person.h"


typedef struct{
    struct person_t* person;//does not need to be the first 
    unsigned int credits;
}student_t;


student_t* student_new(){
    return (student_t*) malloc(sizeof(student_t));
}

void student_ctor(student_t* student, const char* fName, const char* lName,unsigned int credits){
    student->person = person_new();
    person_ctor(student->person, fName, lName);
    student->credits = credits;
    

}
void student_dtor(student_t* student){

    person_dtor(student->person);
    free(student->person);
}
unsigned int student_get_credits(student_t* student){
    return student->credits;
}


void student_get_first_name(student_t* student, char* buffer){
    person_get_first_name(student->person,buffer);
}
void student_get_last_name(student_t* student, char* buffer){
    person_get_last_name(student->person,buffer);
}

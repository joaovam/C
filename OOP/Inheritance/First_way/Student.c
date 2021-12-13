#include <string.h>
#include <stdlib.h>
#include "Person.h"
#include "Person_p.h"


typedef struct{
    person_t person;//needs to be the first item in the struct
    unsigned int credits;
}student_t;


student_t* student_new(){
    return (student_t*) malloc(sizeof(student_t));
}

void student_ctor(student_t* student, const char* fName, const char* lName,unsigned int credits){
    person_ctor((struct person_t*)student, fName, lName);
    student->credits = credits;
    

}
void student_dtor(student_t* student){
    person_dtor((struct person_t*)student);
}
unsigned int student_get_credits(student_t* student){
    return student->credits;
}


/*In this way the student, who inherit person, has 
access to all the person's definitions*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Person.h"
#include "Student.h"
int main(int argc,char** argv){

    struct student_t* student = student_new();
    student_ctor(student,"joao","amorim",92);
    struct person_t* person  = (struct person_t*) student;
    char buffer[32];
    person_get_first_name(person,buffer);
    
    printf("First Name: %s\n",buffer);
    person_get_last_name(person,buffer);
    printf("Last Name: %s\n",buffer);
    printf("Credits: %d\n",student_get_credits(student));
    student_dtor(student);
    //free(person);
    free(student);
    

}
/*In this way the student, who inherit person, does not has 
access to all the person's definitions.However the student needs to offer 
all the person's behaviour functions */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Person.h"
#include "Student.h"
int main(int argc,char** argv){

    struct student_t* student = student_new();
    student_ctor(student,"joao","amorim",92);
    
    char buffer[32];
    student_get_first_name(student,buffer);
    
    printf("First Name: %s\n",buffer);
    student_get_last_name(student,buffer);
    printf("Last Name: %s\n",buffer);
    printf("Credits: %d\n",student_get_credits(student));
    student_dtor(student);
    //free(person);
    free(student);
    return 0;    

}
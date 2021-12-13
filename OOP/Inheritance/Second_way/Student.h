#ifndef STUDENT_H
#define STUDENT_H

struct student_t;
struct student_t* student_new();
void student_ctor(struct student_t*, const char*, const char*,unsigned int);

void student_dtor(struct student_t*);
unsigned int student_get_credits(struct student_t*);

//It has to repeat all the behaviour functions declared in the Person class
void student_get_first_name(struct student_t*, char*);
void student_get_last_name(struct student_t*, char*);

#endif

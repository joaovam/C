#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Animal.h"
#include "Cat.h"
#include "Duck.h"

int main(int argc,char** argv){
    //Firstly we have to alocate the memory necessary for the instances
    struct animal_t* animal = animal_new(animal);
    struct cat_t* cat = cat_new(cat);
    struct duck_t* duck = duck_new(duck);

    //Then we call the constructors to build our instances
    animal_ctor(animal);
    cat_ctor(cat);
    duck_ctor(duck);

  	animal_sound(animal);
    animal_sound((struct animal_t*) cat);
    animal_sound((struct animal_t*) duck);
    
    
    animal_dtor(animal);
    cat_dtor(cat);
    duck_dtor(duck);
    free(animal);
    free(cat);
    free(duck);
}
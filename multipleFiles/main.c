#include<stdio.h>
#include "average.h"

int main(int argc,char** argv){

    int array[5];

    array[0] = 10;
    array[1] = 3;
    array[2] = 5;
    array[3] = -8;
    array[4] = 9;

    double average = avg(array, 5, NORMAL);
    printf("Average = %.2f\n",average);

    average = avg(array, 5, SQUARED);
    printf("Squared average = %.2f\n",average);

}
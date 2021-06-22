#include<stdio.h>
#define SIZE 5

int main(int argc, char**argv){
    int arr[SIZE];
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    arr[3] = 4;
    arr[4] = 5;

    
    printf("Without using pointer\n");

    for(int i = 0;i<SIZE;i++){
        printf("%d\n",arr[i]);
    }

    int *ptr = 0;
    ptr = arr;
    

    printf("Using pointers\n");

    while(ptr!=&arr[SIZE-1]){
        ptr++;
        printf("%d\n",*ptr);
        

    }


}
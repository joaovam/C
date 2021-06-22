#include <stdio.h>
int sum(int x, int y){
    return x+y;

}

int sub(int x, int y){
    return x-y;
}



int main(int argc, char** argv){
    int (*func_ptr) (int,int);
    func_ptr = &sum;
    int number = func_ptr(3,1);
    printf("Number = %d\n",number);



    func_ptr = &sub;
    number = func_ptr(3,1);
    printf("Number = %d\n",number);


    return 0;
}
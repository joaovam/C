#include <stdio.h>

void printBytes(size_t size, void* data){

    unsigned char* byte = data;
    char delimiter = ' ';

    for (size_t i = 0;i<size;i++){
        printf("%c 0x%x\t",delimiter,*byte);
        byte++;
        delimiter = ',';
    }
    printf("\n");

}


int main(int argc,char** argv){
    int a = 1;
    double b = 8.6;

    printBytes(sizeof(int),&a);
    printBytes(sizeof(double),&b);
    return 0;
}
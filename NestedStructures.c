#include <stdio.h>

typedef struct{
    int x;
    int y;

}point_t;

typedef struct {
    point_t center;
    float radius;

}circle_t;

int main(int argc, char** argv){

    circle_t c;

    point_t *p1 = NULL;
    circle_t *p2 = NULL;
    int *p3 = NULL;

    p1 = (point_t*)&c;
    p2 = &c;
    p3 = (int*)&c;
    

    printf("Pointer Value = %p\n",p1);
    printf("Pointer Value = %p\n",p2);
    printf("Pointer Value = %p\n",p3);

    return 0;

}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* first_thread(void* arg){
    printf("Hello World from first Thread!\n");
    return NULL;
}

int main(int argc,char**argv){
    pthread_t thread;
    int result = pthread_create(&thread,NULL,first_thread,NULL);//should return 0 if everything goes ok
    if(result){
        printf("Something went wrong while creating thread! Error Number: %d\n",result);
        exit(1);
    }

    result = pthread_join(thread,NULL);
    if(result){
        printf("Something went wrong while joining thread! Error Number: %d\n",result);
        exit(2);
    }
    return 0;

}//Remember to use gcc -lpthread to compile
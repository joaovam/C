
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CHECK_RESULT(result) if(result){printf("A Pthread Error Ocurred!\n");exit(1);}


int TRUE = 1;
int FALSE = 0;

char* shared_array;

size_t shared_array_len;

pthread_barrier_t alloc_barrier;//barrier used to control if the array has been allocated
pthread_barrier_t filled_barrier;//controls if the array has already been filled
pthread_barrier_t done_barrier;//controls if the process is done and if it can be freed

void* alloc_thread_body(void* arg){
    shared_array_len = 20;
    shared_array = (char*)malloc(sizeof(char)*shared_array_len);
    pthread_barrier_wait(&alloc_barrier);
    return NULL;
}

void* filler_thread_body(void* arg){
    pthread_barrier_wait(&alloc_barrier);
    int even = *(int*)arg;
    char c = 'a';
    size_t start_index = 1;
    if(even){
        c = 'Z';
        start_index = 0;
    }
    for(size_t i = start_index ; i < shared_array_len ; i+=2){
        shared_array[i] = even ? c-- : c++;
    }
    shared_array[shared_array_len-1] = '\0';
    pthread_barrier_wait(&filled_barrier);
    return NULL;
}

void* printer_thread_body(void* arg){
    pthread_barrier_wait(&filled_barrier);
    printf(">>%s\n",shared_array);
    pthread_barrier_wait(&done_barrier);
    return NULL;
}

void* dealloc_thread_body(void* arg){
    pthread_barrier_wait(&done_barrier);
    free(shared_array);
    pthread_barrier_destroy(&alloc_barrier);
    pthread_barrier_destroy(&filled_barrier);
    pthread_barrier_destroy(&done_barrier);
    return NULL;

    

}

int main(int argc,char** argv){
    shared_array = NULL;
    pthread_barrier_init(&alloc_barrier,NULL,3);
    pthread_barrier_init(&filled_barrier,NULL,3);
    pthread_barrier_init(&done_barrier,NULL,2);

    pthread_t alloc_thread;
    pthread_t even_filler_thread;
    pthread_t odd_filler_thread;
    pthread_t print_thread;
    pthread_t dealloc_thread;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    CHECK_RESULT(pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED));
    

    CHECK_RESULT(pthread_create(&alloc_thread,&attr,alloc_thread_body,NULL));
    
    CHECK_RESULT(pthread_create(&even_filler_thread,&attr,filler_thread_body,&TRUE));
    
    CHECK_RESULT(pthread_create(&odd_filler_thread,&attr,filler_thread_body,&FALSE));

    CHECK_RESULT(pthread_create(&print_thread,&attr,printer_thread_body,NULL));
    
    CHECK_RESULT(pthread_create(&dealloc_thread,&attr,dealloc_thread_body,NULL));
    

    pthread_exit(NULL);
    return 0;
}   
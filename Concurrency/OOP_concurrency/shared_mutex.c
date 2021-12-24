#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>

#include "shared_mem.h"
#define TRUE 1
#define FALSE 0
typedef struct
{
    struct shared_mem_t *shm;
    pthread_mutex_t *ptr;
} shared_mutex_t;

shared_mutex_t *shared_mutex_new()
{
    shared_mutex_t *obj = (shared_mutex_t *)malloc(sizeof(shared_mutex_t));
    obj->shm = shared_mem_new();
    return obj;
}

void shared_mutex_delete(shared_mutex_t *obj)
{
    shared_mem_delete(obj->shm);
    free(obj);
}

void shared_mutex_ctor(shared_mutex_t *obj, const char *name)
{
    shared_mem_ctor(obj->shm, name, sizeof(pthread_mutex_t));
    obj->ptr = (pthread_mutex_t *)shared_mem_getptr(obj->shm);
    if (shared_mem_isowner(obj->shm))
    {
        pthread_mutexattr_t attr;
        int ret = -1;
        if ((ret = pthread_mutexattr_init(&attr)))
        {
            fprintf(stderr, "ERROR(%s):Could not initialize mutex attr: %s\n", name, strerror(ret));
            exit(1);
        }
#if !defined(__APPLE__)
        if ((ret = pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST)))
        {
            fprintf(stderr, "ERROR(%s):Could not set mutex attr robust: %s\n", name, strerror(ret));
            exit(1);
        }
#endif
        if ((ret = pthread_mutex_init(obj->ptr, &attr)))
        {
            fprintf(stderr, "ERROR(%s):Could not initialize mutex: %s\n", name, strerror(ret));
            exit(1);
        }
        if ((ret = pthread_mutexattr_destroy(&attr)))
        {
            fprintf(stderr, "ERROR(%s):Could not destroy attr: %s\n", name, strerror(ret));
            exit(1);
        }
    }
}

void shared_mutex_dtor(shared_mutex_t *obj)
{

    if (shared_mem_isowner(obj->shm))
    {
        int ret = -1;
        if ((ret = pthread_mutex_destroy(obj->ptr)))
        {
            fprintf(stderr, "WARN:Could not destroy mutex: %s\n", strerror(ret));
        }
    }
    shared_mem_dtor(obj->shm);
}

pthread_mutex_t *shared_mutex_getptr(shared_mutex_t *obj)
{
    return obj->ptr;
}

#if !defined(__APPLE__)

void shared_mutex_make_consistent(shared_mutex_t *obj)
{
    int ret = -1;
    if ((ret = pthread_mutex_consistent(obj->ptr)))
    {
        fprintf(stderr, "ERROR:Could not make mutex consistent: %s\n", strerror(ret));
        exit(1);
    }
}

#endif

void shared_mutex_lock(shared_mutex_t *obj)
{
    int ret = -1;
    if ((ret = pthread_mutex_lock(obj->ptr)) < 0)
    {
#if !defined(__APPLE__)
        if (ret = EOWNERDEAD)
        {
            fprintf(stderr, "WARN:The owner of the mutex is dead: %s\n", strerror(ret));
            shared_mutex_make_consistent(obj);
            fprintf(stdout, "INFO:I am the new owner\n");
            shared_mem_setowner(obj->shm, TRUE);
            return;
        }
#endif
        fprintf(stderr, "ERROR: Could not lock the mutex: %s\n", strerror(ret));
        exit(1);
    }
}

void shared_mutex_unlock(shared_mutex_t *obj)
{
    int ret = -1;
    if ((ret = pthread_mutex_unlock(obj->ptr)))
    {
        fprintf(stderr, "ERROR: Could not unlock the mutex: %s\n", strerror(ret));
        exit(1);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>

#define TRUE 1
#define FALSE 0

typedef int32_t bool_t;

bool_t owner_process_set = FALSE;
bool_t owner_process = FALSE;

typedef struct
{
    char *name;
    int shm_fd;
    void *map_ptr;
    char *ptr;
    size_t size;

} shared_mem_t;

shared_mem_t *shared_mem_new()
{
    return (shared_mem_t *)malloc(sizeof(shared_mem_t));
}

void shared_mem_delete(shared_mem_t *obj)
{
    free(obj->name);
    free(obj);
}

void shared_mem_ctor(shared_mem_t *obj, const char *name, size_t size)
{
    obj->size = size;
    obj->name = malloc(strlen(name) + 1);

    strcpy(obj->name, name);

    obj->shm_fd = shm_open(obj->name, O_RDWR, 0600);
    if (obj->shm_fd >= 0)
    {
        if (!owner_process_set)
        {
            owner_process = FALSE;
            owner_process_set = FALSE;
        }
        fprintf(stdout, "The shared memory %s is opened\n", obj->name);
    }
    else if (errno == ENOENT)
    {
        fprintf(stderr, "WARN: The shared memory %s does not exist\n", obj->name);
        obj->shm_fd = shm_open(obj->name, O_CREAT | O_RDWR, 0600);

        if (!owner_process_set)
        {
            owner_process = TRUE;
            owner_process_set = TRUE;
        }
        fprintf(stdout, "The shared memory %s is created and opened.\n", obj->name);
        if (ftruncate(obj->shm_fd, obj->size) < 0)
        {
            fprintf(stderr, "ERROR(%s): Could not truncate:%s ", obj->name, strerror(errno));
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "ERROR:(%s): Failed to create shared memory: %s", obj->name, strerror(errno));
        exit(1);
    }
    obj->map_ptr = mmap(0, obj->size, PROT_READ | PROT_WRITE, MAP_SHARED, obj->shm_fd, 0);
    if (obj->map_ptr == MAP_FAILED)
    {
        fprintf(stderr, "ERROR(%s): Failed to map shared memory region: %s", obj->name, strerror(errno));
        exit(1);
    }
    obj->ptr = (char *)obj->map_ptr;
}

void shared_mem_dtor(shared_mem_t *obj)
{
    if (munmap(obj->map_ptr, obj->size) < 0)
    {
        fprintf(stderr, "ERROR(%s):Could not unmap memory region: %s\n", obj->name, strerror(errno));
        exit(1);
    }
    fprintf(stdout, "Shared memory %s is unmapped\n", obj->name);

    if (close(obj->shm_fd) < 0)
    {
        fprintf(stderr, "ERROR(%s):Could not close memory region: %s\n", obj->name, strerror(errno));
        exit(1);
    }
    if (owner_process)
    {
        if (shm_unlink(obj->name) < 0)
        {
            fprintf(stderr, "ERROR(%s):Could not unlink memory region: %s\n", obj->name, strerror(errno));
            exit(1);
        }
        fprintf(stdout, "Shared memory %s is deleted\n", obj->name);
    }
}

char *shared_mem_getptr(shared_mem_t *obj)
{
    return obj->ptr;
}

bool_t shared_mem_isowner(shared_mem_t *obj)
{
    return owner_process;
}

void shared_mem_setowner(shared_mem_t *obj, bool_t is_owner)
{
    owner_process = is_owner;
}
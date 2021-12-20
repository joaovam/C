#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>

#define SHARED_MEM_SIZE 4

int shared_fd = -1;
int32_t *counter = NULL;

sem_t *semaphore = NULL;

void init_control_mechanism()
{

    semaphore = sem_open("/sem0", O_CREAT | O_EXCL, 0600, 1); // using a named semaphore to process synchronization
    if (semaphore == SEM_FAILED)
    {
        fprintf(stderr, "ERROR:Opening the semaphore failed:%s\n", strerror(errno));
        exit(1);
    }
    fprintf(stdout, "Semaphore created succesfully\n");
}

void shutdown_control_mechanism()
{
    if (sem_close(semaphore) < 0)
    {
        fprintf(stderr, "ERROR:Closing the sempahore failed:%s\n", strerror(errno));
        exit(1);
    }

    if (sem_unlink("/sem0") < 0)
    {
        fprintf(stderr, "ERROR: unlinking failed: %s\n", strerror(errno));
        exit(1);
    }
}

void init_shared_resource()
{
    shared_fd = shm_open("/shm0", O_CREAT | O_RDWR, 0600);
    if (shared_fd < 0)
    {
        fprintf(stderr, "ERROR: creating shared resource failed:%s\n", strerror(errno));
        exit(1);
    }
    fprintf(stdout, "Creation of shared resource went succesfully. FD:%d\n", shared_fd);

    if (ftruncate(shared_fd, SHARED_MEM_SIZE * sizeof(char)) < 0)
    {

        fprintf(stderr, "ERROR: Truncation failed: %s\n",

                strerror(errno));

        exit(1);
    }

    fprintf(stdout, "The memory region is truncated.\n");
}

void shutdown_shared_resource()
{
    if (shm_unlink("/shm0") < 0)
    {
        fprintf(stderr, "ERROR:unlinking shared resource failed:%s\n", strerror(errno));
        exit(1);
    }
}

void inc_counter()
{

    usleep(1); // we are using sleep to have the chance of creating many different interleavings
    if (sem_wait(semaphore) < 0)
    {
        fprintf(stderr, "ERROR:putting semaphore in wait state: %d\n", strerror(errno));
    }
    int32_t temp = *counter;
    usleep(1);
    temp++;
    usleep(1);
    *counter = temp;
    if (sem_post(semaphore) < 0)
    {
        fprintf(stderr, "ERROR:opening semaphore: %d\n", strerror(errno));
    }

    usleep(1);
}

int main(int argc, char **argv)
{
    init_shared_resource();
    init_control_mechanism();

    // Map the shared memory and initialize the counter

    void *map = mmap(0, SHARED_MEM_SIZE, PROT_WRITE,

                     MAP_SHARED, shared_fd, 0);

    if (map == MAP_FAILED)
    {

        fprintf(stderr, "ERROR: Mapping failed: %s\n",

                strerror(errno));

        return 1;
    }

    counter = (int32_t *)map;

    *counter = 0;

    // Fork a new process
    inc_counter();
    pid_t pid = fork();

    if (pid)
    {
        inc_counter();
        fprintf(stdout, "The parent process sees the counter as: %d\n", *counter);
        int status = -1;
        wait(&status);
        fprintf(stdout, "The child process finished with status %d.\n",
                status);
    }
    else
    {
        inc_counter();
        fprintf(stdout, "The child process sees the counter as: %d\n", *counter);
    }

    if (munmap(counter, SHARED_MEM_SIZE) < 0)
    {
        fprintf(stderr, "ERROR:Unmapping Failed: %s\n", strerror(errno));
        return 1;
    }
    if (close(shared_fd) < 0)
    {
        fprintf(stderr, "ERROR:Closing shared memory fd filed:%s\n", strerror(errno));
        return 1;
    }

    if (pid)
    {
        shutdown_control_mechanism();
        shutdown_shared_resource();
    }
    return 0;
}
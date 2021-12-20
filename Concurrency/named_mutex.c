#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>

typedef u_int16_t bool_t;

#define TRUE 1
#define FALSE 0

#define MUTEX_SHM_NAME "/mutex0"
#define SHM_NAME "/shm0"

int cancel_flag_shm_fd = -1;
int mutex_shm_fd = -1;
int counter_shm_fd = -1;

bool_t *cancel_flag = NULL;
pthread_mutex_t *mutex = NULL;
int32_t *counter = NULL;

bool_t cancel_flag_shm_owner = FALSE;
bool_t mutex_owner = FALSE;
bool_t counter_owner = FALSE;

void init_shared_resource()
{
    cancel_flag_shm_fd = shm_open(SHM_NAME, O_RDWR, 0600);
    if (cancel_flag_shm_fd >= 0)
    {
        cancel_flag_shm_owner = FALSE;
        fprintf(stdout, "Shared memory object already exists, opening it\n");
    }
    else if (errno == ENOENT)
    {
        fprintf(stderr, "WARN: the shared memory object does not exist.\n");
        fprintf(stderr, "Creating shared memory object...\n");
        cancel_flag_shm_fd = shm_open(SHM_NAME, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (cancel_flag_shm_fd >= 0)
        {
            cancel_flag_shm_owner = TRUE;
            fprintf(stdout, "Shared memory object is created.\n");
        }
        else
        {
            fprintf(stderr, "ERROR:Failed to create shared memory object: %s.\n", strerror(errno));
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "ERROR:Error while creating shared resource for the cancel flag: %s\n", strerror(errno));
        exit(1);
    }
    if (cancel_flag_shm_owner)
    {
        if (ftruncate(cancel_flag_shm_fd, sizeof(bool_t)) < 0)
        {
            fprintf(stderr, "ERROR: error truncating shared memory: %s\n", strerror(errno));
            exit(1);
        }
    }
    fprintf(stdout, "The memory region is truncated.\n");

    void *map = mmap(0, sizeof(bool_t), PROT_WRITE, MAP_SHARED, cancel_flag_shm_fd, 0);

    if (map == MAP_FAILED)
    {
        fprintf(stderr, "ERROR: Mapping failed: %s\n", strerror(errno));
        exit(1);
    }
    cancel_flag = (bool_t *)map;
    if (cancel_flag_shm_owner)
    {
        *cancel_flag = FALSE;
    }
}

void shutdown_shared_resource()
{
    // cancel flag
    // mutex

    if (munmap(cancel_flag, sizeof(bool_t)) < 0)
    {
        fprintf(stderr, "ERROR: Unmaping failed: %s\n", strerror(errno));
        exit(1);
    }

    if (close(cancel_flag_shm_fd) < 0)
    {
        fprintf(stderr, "ERROR:Problem while closing shm: %s\n", strerror(errno));
        exit(1);
    }

    if (cancel_flag_shm_owner)
    {
        sleep(1);
        if (shm_unlink(SHM_NAME) < 0)
        {
            fprintf(stderr, "ERROR:Not able to unlink: %s\n", strerror(errno));
            exit(1);
        }
    }
}

void init_increment_shared_resource()
{
    counter_shm_fd = shm_open("/shm_counter", O_RDWR, 0600);
    if (counter_shm_fd >= 0)
    {
        fprintf(stdout, "Shared resource for counter is opened.\n");
    }
    else if (errno == ENOENT)
    {

        fprintf(stderr, "WARN:The shared resource does not exist\n");
        fprintf(stdout, "Creating shared resource...\n");

        counter_shm_fd = shm_open("/shm_counter", O_RDWR | O_EXCL | O_CREAT, 0600);

        if (counter_shm_fd < 0)
        {
            fprintf(stderr, "Could not create shared resource: %s", strerror(errno));
            exit(1);
        }
        counter_owner = TRUE;
    }
    else
    {
        fprintf(stderr, "Could not open shared resource: %s", strerror(errno));
        exit(1);
    }

    if (counter_owner)
    {
        if (ftruncate(counter_shm_fd, sizeof(int32_t)) < 0)
        {
            fprintf(stderr, "ERROR: could not truncate shared memory:%s\n", strerror(errno));
            exit(1);
        }
        fprintf(stdout, "shared memory truncated\n");
    }
    void *map = mmap(0, sizeof(int32_t), PROT_WRITE | PROT_READ, MAP_SHARED, counter_shm_fd, 0);
    if (map == MAP_FAILED)
    {
        fprintf(stderr, "ERROR: could not map shared memory:%s\n", strerror(errno));
        exit(1);
    }
    counter = (int32_t *)map;
}

void increment()
{
    pthread_mutex_lock(mutex);
    int32_t temp = *counter;
    temp++;
    *counter = temp;
    pthread_mutex_unlock(mutex);
}

void init_control_mechanism()
{

    mutex_shm_fd = shm_open("/mutex0", O_RDWR, 0600);
    if (mutex_shm_fd >= 0)
    {
        // mutex exists
        mutex_owner = FALSE;
        fprintf(stdout, "Mutex opened succesfully\n");
    }
    else if (errno == ENOENT)
    {
        // mutex does not exist
        mutex_shm_fd = shm_open("/mutex0", O_CREAT | O_RDWR | O_EXCL, 0600);
        if (mutex_shm_fd >= 0)
        {
            mutex_owner = TRUE;
            fprintf(stdout, "Mutex shared resource created\n");
        }
        else
        {
            fprintf(stderr, "ERROR: could not create shared resource:%s\n", strerror(errno));
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "ERROR: could not create shared resource:%s\n", strerror(errno));
        exit(1);
    }
    if (mutex_owner)
    {
        if (ftruncate(mutex_shm_fd, sizeof(pthread_mutex_t)) < 0)
        {
            fprintf(stderr, "Mutex shared memory could not be truncated:%s\n ", strerror(errno));
            exit(1);
        }
    }
    void *map = mmap(0, sizeof(pthread_mutex_t), PROT_WRITE | PROT_READ, MAP_SHARED, mutex_shm_fd, 0);

    if (map == MAP_FAILED)
    {
        fprintf(stderr, "ERROR:Mapping failed:%s", strerror(errno));
        exit(1);
    }
    mutex = (pthread_mutex_t *)map;
    if (mutex_owner)
    {
        int ret = -1;
        pthread_mutexattr_t attr;
        if ((ret = pthread_mutexattr_init(&attr)))
        {
            fprintf(stderr, "ERROR: Initializing mutex attr failed: %s\n", strerror(ret));
            exit(1);
        }
        if ((ret = pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)))
        {
            fprintf(stderr, "ERROR: setting  mutex attr failed: %s\n", strerror(ret));
            exit(1);
        }
        if ((ret = pthread_mutex_init(mutex, &attr)))
        {
            fprintf(stderr, "ERROR: creating mutex failed: %s\n", strerror(ret));
            exit(1);
        }

        if ((ret = pthread_mutexattr_destroy(&attr)))
        {
            fprintf(stderr, "ERROR: Destruction of mutex attr failed: %s\n", strerror(ret));
            exit(1);
        }
    }
}

void shutdown_control_mechanism()
{
    sleep(1);
    if (mutex_owner)
    {
        int ret = -1;
        if ((ret = pthread_mutex_destroy(mutex)))
        {
            fprintf(stderr, "WARN:Destruction of the mutex failed: %s\n", strerror(ret));
        }
    }
    if (munmap(mutex, sizeof(pthread_mutex_t)) < 0)
    {
        fprintf(stderr, "ERROR: Unmapping the mutex failed: %s\n", strerror(errno));
        exit(1);
    }
    if (close(mutex_shm_fd) < 0)
    {
        fprintf(stderr, "ERROR:Closing the mutex failed: %s\n", strerror(errno));
        exit(1);
    }
    if (mutex_owner)
    {
        if (shm_unlink(MUTEX_SHM_NAME) < 0)
        {
            fprintf(stderr, "ERROR: Unlinking the mutex failed: %s\n", strerror(errno));
            exit(1);
        }
    }
}

void shutdown_counte_shared_resource()
{
    if (munmap(counter, sizeof(int32_t)) < 0)
    {
        fprintf(stderr, "ERROR:Could not unmap shared resource:%s\n", strerror(errno));
    }
    if (close(counter_shm_fd) < 0)
    {
        fprintf(stderr, "Could not close shared resource: %s\n", strerror(errno));
        exit(1);
    }
    fprintf(stdout, "Shared resource closed\n");
    if (counter_owner)
    {
        sleep(1);
        if (shm_unlink("/shm_counter") < 0)
        {
            fprintf(stderr, "ERROR:Could not unlink shared resource:%s\n", strerror(errno));
            exit(1);
        }
    }
}

bool_t is_canceled()
{
    pthread_mutex_lock(mutex);
    bool_t temp = *cancel_flag;
    pthread_mutex_unlock(mutex);
    return temp;
}

void cancel()
{
    pthread_mutex_lock(mutex);
    *cancel_flag = TRUE;
    pthread_mutex_unlock(mutex);
}

void sigint_handler(int signo)
{
    fprintf(stdout, "\nHandling INT signal: %d...\n", signo);
    cancel();
}

int main(int argc, char **argv)
{
    signal(SIGINT, sigint_handler);

    init_shared_resource();
    init_control_mechanism();
    init_increment_shared_resource();

    while (!is_canceled())
    {
        fprintf(stdout, "Working....\n");

        if (counter_owner)
        {
            fprintf(stdout, "Counter value:%d\n", *counter);
        }
        else
        {
            increment();
        }
        sleep(1);
    }
    fprintf(stdout, "Cancel signal is received.\n");
    shutdown_shared_resource();
    shutdown_control_mechanism();
    shutdown_counte_shared_resource();
    return 0;
}
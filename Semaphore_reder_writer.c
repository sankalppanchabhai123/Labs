#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define OPERATIONS 5

// Shared data
int shared_data = 0;

// Semaphores
sem_t writer_sem, reader_sem;

// Writer thread function
void *writer(void *arg)
{
    int writer_id = *(int *)arg;

    for (int i = 0; i < OPERATIONS; i++)
    {
        // Wait for writer semaphore
        sem_wait(&writer_sem);

        // Critical section - Writing
        shared_data++;
        printf("Writer %d: Writing data = %d\n", writer_id, shared_data);
        sleep(1); // Simulate writing time

        // Signal reader semaphore
        sem_post(&reader_sem);
    }

    return NULL;
}

// Reader thread function
void *reader(void *arg)
{
    int reader_id = *(int *)arg;

    for (int i = 0; i < OPERATIONS; i++)
    {
        // Wait for reader semaphore
        sem_wait(&reader_sem);

        // Critical section - Reading
        printf("Reader %d: Reading data = %d\n", reader_id, shared_data);
        sleep(1); // Simulate reading time

        // Signal writer semaphore
        sem_post(&writer_sem);
    }

    return NULL;
}

int main()
{
    pthread_t writer_thread, reader_thread;
    int writer_id = 1, reader_id = 1;

    printf("=== Single Reader-Single Writer Synchronization ===\n");

    // Initialize semaphores
    sem_init(&writer_sem, 0, 1); // Writer starts first
    sem_init(&reader_sem, 0, 0); // Reader waits initially

    // Create threads
    pthread_create(&writer_thread, NULL, writer, &writer_id);
    pthread_create(&reader_thread, NULL, reader, &reader_id);

    // Wait for threads to complete
    pthread_join(writer_thread, NULL);
    pthread_join(reader_thread, NULL);

    // Destroy semaphores
    sem_destroy(&writer_sem);
    sem_destroy(&reader_sem);

    printf("All operations completed!\n");
    return 0;
}
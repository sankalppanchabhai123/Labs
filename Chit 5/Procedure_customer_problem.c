#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SIZE 5

int buffer[SIZE];
int count = 0, in = 0, out = 0;

sem_t empty, full, mutex;

void *producer(void *arg)
{
    for (int i = 1; i <= 10; i++)
    {
        sem_wait(&empty);
        sem_wait(&mutex);

        // Produce
        buffer[in] = i;
        printf("Produced: %d\n", i);
        in = (in + 1) % SIZE;
        count++;

        sem_post(&mutex);
        sem_post(&full);
        sleep(1);
    }
    return NULL;
}

void *consumer(void *arg)
{
    for (int i = 1; i <= 10; i++)
    {
        sem_wait(&full);
        sem_wait(&mutex);

        // Consume
        int item = buffer[out];
        printf("Consumed: %d\n", item);
        out = (out + 1) % SIZE;
        count--;

        sem_post(&mutex);
        sem_post(&empty);
        sleep(2);
    }
    return NULL;
}

int main()
{
    pthread_t p, c;

    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
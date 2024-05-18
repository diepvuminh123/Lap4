#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_ITEMS 1  
#define THREADS 1 // 1 producer and 1 consumer
#define LOOPS 3 * MAX_ITEMS // variable

// Initiates shared buffer
int buffer[MAX_ITEMS];
int fill = 0;
int use = 0;

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

void put(int value) {
    buffer[fill] = value; // line f1
    fill = (fill + 1) % MAX_ITEMS; // line f2
}

int get() {
    int tmp = buffer[use]; // line g1
    buffer[use] = -1;    //clean the item
    use = (use + 1) % MAX_ITEMS; // line g2
    return tmp;
}

void *producer(void *arg) {
    int i;
    int tid = (intptr_t)arg;
    for (i = 0; i < LOOPS; i++) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        put(i); // line P2
        printf("Producer %d put data %d\n", tid, i);
        pthread_mutex_unlock(&mutex);
        // sleep(1);
        sem_post(&full);
        sleep(1);
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int i, tmp = 0;
    int tid = (intptr_t)arg;
    while (tmp != -1) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        tmp = get(); // line C2
        printf("Consumer %d get data %d\n", tid, tmp);
        pthread_mutex_unlock(&mutex);
        // sleep(1);
        sem_post(&empty);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    int i, j;
    int tid[THREADS];
    pthread_t producers[THREADS];
    pthread_t consumers[THREADS];

    sem_init(&empty, 0, MAX_ITEMS);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < THREADS; i++) {
        tid[i] = i;
        // Create producer thread
        pthread_create(&producers[i], NULL, producer, (void *)(intptr_t)tid[i]);
        // Create consumer thread
        pthread_create(&consumers[i], NULL, consumer, (void *)(intptr_t)tid[i]);
    }

    for (i = 0; i < THREADS; i++) {
        pthread_join(producers[i], NULL);
        pthread_join(consumers[i], NULL);
    }

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}

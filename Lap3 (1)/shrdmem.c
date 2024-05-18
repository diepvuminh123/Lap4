#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int MAX_COUNT = 1e9;
static int count = 0;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

void* fcount(void* sid) {
    int i;
    pthread_mutex_lock(&count_mutex); // Lock 
    for (i = 0; i < MAX_COUNT; i++) {
        count = count + 1;
    }
    printf("Thread %s: holding %d\n", (char*)sid, count);
    pthread_mutex_unlock(&count_mutex); // Unlock 
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    /* Create independent threads each of which will execute function */
    pthread_create(&thread1, NULL, &fcount, "1");
    pthread_create(&thread2, NULL, &fcount, "2");

    // Wait for thread1 to finish
    pthread_join(thread1, NULL);
    // Wait for thread2 to finish
    pthread_join(thread2, NULL);
    return 0;
}
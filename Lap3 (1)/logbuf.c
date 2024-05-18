#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_LOG_LENGTH 10
#define MAX_BUFFER_SLOT 6

char *logbuf[MAX_BUFFER_SLOT];
int current_slot = 0;
pthread_mutex_t lock;

// struct _args
// {
//   unsigned int interval;
// }; 

int wrlog(char* new_data) {
    pthread_mutex_lock(&lock);

    if (current_slot >= MAX_BUFFER_SLOT) {
        pthread_mutex_unlock(&lock);
        return -1; // Buffer is full
    }

    logbuf[current_slot] = malloc(MAX_LOG_LENGTH * sizeof(char));
    strncpy(logbuf[current_slot], new_data, MAX_LOG_LENGTH);
    current_slot++;

    pthread_mutex_unlock(&lock);
    return 0;
}

int flushlog() {
    pthread_mutex_lock(&lock);

    for (int i = 0; i < current_slot; i++) {
        printf("Slot %d: %s\n", i, logbuf[i]);
        free(logbuf[i]);
    }

    current_slot = 0;

    pthread_mutex_unlock(&lock);
    return 0;
}

void* logger(void* arg) {
    char** data = (char**)arg;
    for (int i = 0; i < 30; i++) {
        while (wrlog(data[i]) != 0) {
            usleep(1000); // Wait for a slot to become available
        }
    }
    return NULL;
}

int main() {
    pthread_t tid;
    char* data[30] = {"0", "1", "2", "3", "4", "5", "12", "6", "7", "10", "8", "11", "17", "13", "16", "15", "14", "18", "25", "20", "19", "21", "22", "23", "27", "28", "26", "24", "29", "9"};
    
    pthread_mutex_init(&lock, NULL);
    //for(int i = 0; i < 30; i++){
        pthread_create(&tid, NULL, logger, data);
    //}
    

    while (1) {
        flushlog();
        sleep(1); // Flush the log every second
    }
    //for(int i = 0; i < 30; i++)
    pthread_join(tid, NULL);
    pthread_mutex_destroy(&lock);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 1000

// Global sum buffer
long sum_buf = 0;
int* arr_buf;
pthread_mutex_t mtx;

struct range {
    int start;
    int end;
};

void* sum_worker(void *arg) {
    struct range *idx_range = (struct range *)arg;
    long local_sum = 0;

    for (int i = idx_range->start; i <= idx_range->end; i++) {
        local_sum += arr_buf[i];
    }

    pthread_mutex_lock(&mtx);
    sum_buf += local_sum;
    pthread_mutex_unlock(&mtx);

    return NULL;
}

int generate_array_data(int *buf, int array_size, int seed_num) {
    srand(seed_num);
    for (int i = 0; i < array_size; i++) {
        buf[i] = rand() % MAX;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        printf("Usage: %s arrsz tnum [seednum]\n", argv[0]);
        return 1;
    }

    int arrsz = atoi(argv[1]);
    int tnum = atoi(argv[2]);
    int seednum = (argc == 4) ? atoi(argv[3]) : 1;

    arr_buf = malloc(arrsz * sizeof(int));
    generate_array_data(arr_buf, arrsz, seednum);

    pthread_t* tids = malloc(tnum * sizeof(pthread_t));
    struct range* ranges = malloc(tnum * sizeof(struct range));

    int len = arrsz / tnum;
    for (int i = 0; i < tnum; i++) {
        ranges[i].start = i * len;
        ranges[i].end = (i == tnum - 1) ? (arrsz - 1) : ((i + 1) * len - 1);
        pthread_create(&tids[i], NULL, sum_worker, &ranges[i]);
    }

    for (int i = 0; i < tnum; i++) {
        pthread_join(tids[i], NULL);
    }

    printf("Aggregated sum: %ld\n", sum_buf);

    // Single-threaded sum for comparison
    long single_sum = 0;
    for (int i = 0; i < arrsz; i++) {
        single_sum += arr_buf[i];
    }

    printf("Single-threaded sum: %ld\n", single_sum);

    free(arr_buf);
    free(tids);
    free(ranges);

    return 0;
}

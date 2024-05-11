// Group members:
//   Nguyễn Quốc Lộc     - 2252458
//   Trần Tường Khang    - 2252313
//   Diệp Vũ Minh        - 2252472
//   Nguyễn Lê Minh Huân - 2252242


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

#include "forkpool.h"

void my_task(int taskID, int* out) {
    printf("Task %d has been launched.\n", taskID);
    int queue = taskID;
    for (int i = 0; i < 10; i++) {
        queue = (queue % 4) + 3;
    }
    out[taskID] = queue;
}

int main() {
    forkpool_t forkpool;
    forkpool_init(&forkpool);
    forkpool_fork(&forkpool, my_task);
    forkpool_join(&forkpool);

    int result = 0;
    for (int i = 0; i < FORKPOOL_NUM_WORKERS; i++) {
        result += forkpool.task_out[i];
    }
    printf("Output: %d\n", result);

    forkpool_free(&forkpool);

    return 0;
}

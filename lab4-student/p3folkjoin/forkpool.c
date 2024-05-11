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

void forkpool_init(forkpool_t *f) {
    f->shmFD = shm_open("/forkpool_shared", O_CREAT | O_RDWR, 0666);
    if (f->shmFD == -1) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(f->shmFD, FORKPOOL_SHARED_MEM_SIZE) == -1) {
        perror("ftruncate failed");
        exit(EXIT_FAILURE);
    }
    f->taskOut = mmap(NULL, FORKPOOL_SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, f->shmFD, 0);
    if (f->taskOut == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }
}

void forkpool_fork(forkpool_t *f, forkpool_task_fn_t *task) {
    for (int i = 0; i < FORKPOOL_NUM_WORKERS; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            task(i, f->taskOut);
            exit(EXIT_SUCCESS);
        } else {
            f->pids[i] = pid;
        }
    }
}

void forkpool_join(forkpool_t *f) {
    for (int i = 0; i < FORKPOOL_NUM_WORKERS; i++) {
        waitpid(f->pids[i], NULL, 0);
    }
}

void forkpool_free(forkpool_t *f) {
    if (munmap(f->taskOut, FORKPOOL_SHARED_MEM_SIZE) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    if (close(f->shmFD) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    if (shm_unlink("/forkpool_shared") == -1) {
        perror("shm_unlink");
        exit(EXIT_FAILURE);
    }
}

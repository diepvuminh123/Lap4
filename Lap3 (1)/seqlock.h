#ifndef PTHREAD_H
#include <pthread.h>
#endif

typedef struct pthread_seqlock { /* TODO: implement the structure */
   pthread_mutex_t mutex;
   unsigned sequence;
} pthread_seqlock_t;

static inline void pthread_seqlock_init(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   pthread_mutex_init(&rw->mutex, NULL);
   rw->sequence = 0;
}

static inline void pthread_seqlock_wrlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   pthread_mutex_lock(&rw->mutex);
   rw->sequence++;
}

static inline void pthread_seqlock_wrunlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   rw->sequence++;
   pthread_mutex_unlock(&rw->mutex);
}

static inline unsigned pthread_seqlock_rdlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
    return rw->sequence;
}


static inline unsigned pthread_seqlock_rdunlock(pthread_seqlock_t *rw, unsigned seq)
{
   /* TODO: ... */
    return seq != rw->sequence;
}

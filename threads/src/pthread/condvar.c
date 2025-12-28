#include "snthreads/condvar.h"

#if defined(SN_OS_LINUX) || defined(SN_OS_MAC)

#include <pthread.h>
#include <time.h>

typedef struct snMutexPthread {
    pthread_mutex_t mutex;
} snMutexPthread;

typedef struct snCondvarPthread {
    pthread_cond_t cond;
} snCondvarPthread;

SN_STATIC_ASSERT(sizeof(snCondvarPthread) <= sizeof(snCondvar), "snCondvar size insufficient");

bool sn_condvar_init(snCondvar *cv) {
    snCondvarPthread *c = (snCondvarPthread *)cv;
    return pthread_cond_init(&c->cond, NULL) == 0;
}

void sn_condvar_destroy(snCondvar *cv) {
    snCondvarPthread *c = (snCondvarPthread *)cv;
    pthread_cond_destroy(&c->cond);
}

void sn_condvar_wait(snCondvar *cv, snMutex *mutex) {
    snCondvarPthread *c = (snCondvarPthread *)cv;
    snMutexPthread *m = (snMutexPthread *)mutex;

    int r = pthread_cond_wait(&c->cond, &m->mutex);
    SN_ASSERT(r == 0);
}

bool sn_condvar_timed_wait(snCondvar *cv, snMutex *mutex, uint64_t timeout_ns) {
    snCondvarPthread *c = (snCondvarPthread *)cv;
    snMutexPthread *m = (snMutexPthread *)mutex;

    struct timespec ts;
    ts.tv_sec  = timeout_ns / 1000000000ull;
    ts.tv_nsec = timeout_ns % 1000000000ull;

    return pthread_cond_timedwait(&c->cond, &m->mutex, &ts) == 0;
}

void sn_condvar_signal(snCondvar *cv) {
    snCondvarPthread *c = (snCondvarPthread *)cv;
    pthread_cond_signal(&c->cond);
}

void sn_condvar_broadcast(snCondvar *cv) {
    snCondvarPthread *c = (snCondvarPthread *)cv;
    pthread_cond_broadcast(&c->cond);
}

#endif


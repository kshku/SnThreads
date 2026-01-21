#include "snthreads/condvar.h"

#if defined(SN_OS_LINUX) || defined(SN_OS_MAC)

#include <pthread.h>
#include <time.h>

typedef struct snMutexPthread {
    pthread_mutex_t mutex;
} snMutexPthread;

#define MUTEX(mtx) (((snMutexPthread *)(mtx))->mutex) 

typedef struct snCondvarPthread {
    pthread_cond_t cond;
} snCondvarPthread;

#define CONDVAR(condvar) (((snCondvarPthread *)(condvar))->cond)

SN_STATIC_ASSERT(sizeof(snCondvarPthread) <= sizeof(snCondvar), "snCondvar size insufficient");

bool sn_condvar_init(snCondvar *cv) {
    return pthread_cond_init(&CONDVAR(cv), NULL) == 0;
}

void sn_condvar_deinit(snCondvar *cv) {
    pthread_cond_destroy(&CONDVAR(cv));
}

void sn_condvar_wait(snCondvar *cv, snMutex *mutex) {
    int r = pthread_cond_wait(&CONDVAR(cv), &MUTEX(mutex));
    SN_ASSERT(r == 0);
}

bool sn_condvar_timed_wait(snCondvar *cv, snMutex *mutex, uint64_t timeout_ns) {
    struct timespec ts;
    ts.tv_sec  = timeout_ns / 1000000000ull;
    ts.tv_nsec = timeout_ns % 1000000000ull;

    return pthread_cond_timedwait(&CONDVAR(cv), &MUTEX(mutex), &ts) == 0;
}

void sn_condvar_signal(snCondvar *cv) {
    pthread_cond_signal(&CONDVAR(cv));
}

void sn_condvar_broadcast(snCondvar *cv) {
    pthread_cond_broadcast(&CONDVAR(cv));
}

#endif


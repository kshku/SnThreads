#include "snthreads/condvar.h"

#if defined(SN_OS_LINUX) || defined(SN_OS_MAC)

    #include <pthread.h>
    #include <time.h>

typedef struct SnMutexPthread {
    pthread_mutex_t mutex;
} SnMutexPthread;

    #define MUTEX(mtx) (((SnMutexPthread *)(mtx))->mutex)

typedef struct SnCondvarPthread {
    pthread_cond_t cond;
} SnCondvarPthread;

    #define CONDVAR(condvar) (((SnCondvarPthread *)(condvar))->cond)

SN_STATIC_ASSERT(sizeof(SnCondvarPthread) <= sizeof(SnCondvar), "SnCondvar size insufficient");

bool sn_condvar_init(SnCondvar *cv) {
    return pthread_cond_init(&CONDVAR(cv), NULL) == 0;
}

void sn_condvar_deinit(SnCondvar *cv) {
    pthread_cond_destroy(&CONDVAR(cv));
}

void sn_condvar_wait(SnCondvar *cv, SnMutex *mutex) {
    int r = pthread_cond_wait(&CONDVAR(cv), &MUTEX(mutex));
    SN_ASSERT(r == 0);
}

bool sn_condvar_timed_wait(SnCondvar *cv, SnMutex *mutex, uint64_t timeout_ns) {
    struct timespec ts;
    ts.tv_sec = timeout_ns / 1000000000ull;
    ts.tv_nsec = timeout_ns % 1000000000ull;

    return pthread_cond_timedwait(&CONDVAR(cv), &MUTEX(mutex), &ts) == 0;
}

void sn_condvar_signal(SnCondvar *cv) {
    pthread_cond_signal(&CONDVAR(cv));
}

void sn_condvar_broadcast(SnCondvar *cv) {
    pthread_cond_broadcast(&CONDVAR(cv));
}

#endif


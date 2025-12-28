#include "snthreads/mutex.h"

#if defined(SN_OS_LINUX) || defined(SN_OS_MAC)

#include <pthread.h>

typedef struct snMutexPthread {
    pthread_mutex_t mutex;
} snMutexPthread;

SN_STATIC_ASSERT(sizeof(snMutexPthread) <= sizeof(snMutex), "snMutex size is not large enough!");

void sn_mutex_init(snMutex *m) {
    snMutexPthread *mutex = (snMutexPthread *)m;
    pthread_mutex_init(&mutex->mutex, NULL);
}

void sn_mutex_deinit(snMutex *m) {
    snMutexPthread *mutex = (snMutexPthread *)m;
    pthread_mutex_destroy(&mutex->mutex);
}

void sn_mutex_lock(snMutex *m) {
    snMutexPthread *mutex = (snMutexPthread *)m;
    pthread_mutex_lock(&mutex->mutex);
}

bool sn_mutex_try_lock(snMutex *m) {
    snMutexPthread *mutex = (snMutexPthread *)m;
    return pthread_mutex_trylock(&mutex->mutex) == 0;
}

void sn_mutex_unlock(snMutex *m) {
    snMutexPthread *mutex = (snMutexPthread *)m;
    pthread_mutex_unlock(&mutex->mutex);
}

#endif


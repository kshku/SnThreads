#include "snthreads/mutex.h"

#if defined(SN_OS_LINUX) || defined(SN_OS_MAC)

#include <pthread.h>

typedef struct snMutexPthread {
    pthread_mutex_t mutex;
} snMutexPthread;

#define MUTEX(mtx) (((snMutexPthread *)(mtx))->mutex)

SN_STATIC_ASSERT(sizeof(snMutexPthread) <= sizeof(snMutex), "snMutex size is not large enough!");

void sn_mutex_init(snMutex *m) {
    pthread_mutex_init(&MUTEX(m), NULL);
}

void sn_mutex_deinit(snMutex *m) {
    pthread_mutex_destroy(&MUTEX(m));
}

void sn_mutex_lock(snMutex *m) {
    pthread_mutex_lock(&MUTEX(m));
}

bool sn_mutex_try_lock(snMutex *m) {
    return pthread_mutex_trylock(&MUTEX(m)) == 0;
}

void sn_mutex_unlock(snMutex *m) {
    pthread_mutex_unlock(&MUTEX(m));
}

#endif


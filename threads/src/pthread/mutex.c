#include "snthreads/mutex.h"

#if defined(SN_OS_LINUX) || defined(SN_OS_MAC)

    #include <pthread.h>

typedef struct SnMutexPthread {
    pthread_mutex_t mutex;
} SnMutexPthread;

    #define MUTEX(mtx) (((SnMutexPthread *)(mtx))->mutex)

SN_STATIC_ASSERT(sizeof(SnMutexPthread) <= sizeof(SnMutex), "SnMutex size is not large enough!");

void sn_mutex_init(SnMutex *m) {
    pthread_mutex_init(&MUTEX(m), NULL);
}

void sn_mutex_deinit(SnMutex *m) {
    pthread_mutex_destroy(&MUTEX(m));
}

void sn_mutex_lock(SnMutex *m) {
    pthread_mutex_lock(&MUTEX(m));
}

bool sn_mutex_try_lock(SnMutex *m) {
    return pthread_mutex_trylock(&MUTEX(m)) == 0;
}

void sn_mutex_unlock(SnMutex *m) {
    pthread_mutex_unlock(&MUTEX(m));
}

#endif


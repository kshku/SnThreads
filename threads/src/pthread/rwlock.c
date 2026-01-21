#define _GNU_SOURCE
#include "snthreads/rwlock.h"

#if defined(SN_OS_LINUX) || defined(SN_OS_MAC)

#include <pthread.h>

typedef struct snRWLockPthread {
    pthread_rwlock_t rwlock;
} snRWLockPthread;

#define RWLOCK(lock) (((snRWLockPthread *)(lock))->rwlock)

SN_STATIC_ASSERT(sizeof(snRWLockPthread) <= sizeof(snRWLock), "snRWLock size is not large enough!");

bool sn_rwlock_init(snRWLock *rw) {
    return pthread_rwlock_init(&RWLOCK(rw), NULL) == 0;
}

void sn_rwlock_deinit(snRWLock *rw) {
    pthread_rwlock_destroy(&RWLOCK(rw));
}

void sn_rwlock_read_lock(snRWLock *rw) {
    pthread_rwlock_rdlock(&RWLOCK(rw));
}

bool sn_rwlock_try_read_lock(snRWLock *rw) {
    return pthread_rwlock_tryrdlock(&RWLOCK(rw)) == 0;
}

void sn_rwlock_read_unlock(snRWLock *rw) {
    pthread_rwlock_unlock(&RWLOCK(rw));
}

void sn_rwlock_write_lock(snRWLock *rw) {
    pthread_rwlock_wrlock(&RWLOCK(rw));
}

bool sn_rwlock_try_write_lock(snRWLock *rw) {
    return pthread_rwlock_trywrlock(&RWLOCK(rw)) == 0;
}

void sn_rwlock_write_unlock(snRWLock *rw) {
    pthread_rwlock_unlock(&RWLOCK(rw));
}

#endif

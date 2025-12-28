#define _GNU_SOURCE
#include "snthreads/rwlock.h"

#if defined(SN_OS_LINUX) || defined(SN_OS_MAC)

#include <pthread.h>

typedef struct snRWLockPthread {
    pthread_rwlock_t rwlock;
} snRWLockPthread;

SN_STATIC_ASSERT(sizeof(snRWLockPthread) <= sizeof(snRWLock), "snRWLock size is not large enough!");

bool sn_rwlock_init(snRWLock *rw) {
    snRWLockPthread *l = (snRWLockPthread *)rw;
    return pthread_rwlock_init(&l->rwlock, NULL) == 0;
}

void sn_rwlock_deinit(snRWLock *rw) {
    snRWLockPthread *l = (snRWLockPthread *)rw;
    pthread_rwlock_destroy(&l->rwlock);
}

void sn_rwlock_rdlock(snRWLock *rw) {
    snRWLockPthread *l = (snRWLockPthread *)rw;
    pthread_rwlock_rdlock(&l->rwlock);
}

bool sn_rwlock_try_rdlock(snRWLock *rw) {
    snRWLockPthread *l = (snRWLockPthread *)rw;
    return pthread_rwlock_tryrdlock(&l->rwlock) == 0;
}

void sn_rwlock_rdunlock(snRWLock *rw) {
    snRWLockPthread *l = (snRWLockPthread *)rw;
    pthread_rwlock_unlock(&l->rwlock);
}

void sn_rwlock_wrlock(snRWLock *rw) {
    snRWLockPthread *l = (snRWLockPthread *)rw;
    pthread_rwlock_wrlock(&l->rwlock);
}

bool sn_rwlock_try_wrlock(snRWLock *rw) {
    snRWLockPthread *l = (snRWLockPthread *)rw;
    return pthread_rwlock_trywrlock(&l->rwlock) == 0;
}

void sn_rwlock_wrunlock(snRWLock *rw) {
    snRWLockPthread *l = (snRWLockPthread *)rw;
    pthread_rwlock_unlock(&l->rwlock);
}

#endif

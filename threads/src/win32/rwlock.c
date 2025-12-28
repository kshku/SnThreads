#include "snthreads/rwlock.h"

#if defined(SN_OS_WINDOWS)

#include <windows.h>

typedef struct snRWLockWin32 {
    SRWLOCK rwlock;
} snRWLockWin32;

SN_STATIC_ASSERT(sizeof(snRWLockWin32) <= sizeof(snRWLock), "snRWLock size is not large enough!");

bool sn_rwlock_init(snRWLock *rw) {
    snRWLockWin32 *l = (snRWLockWin32 *)rw;
    InitializeSRWLock(&l->rwlock);
    return true;
}

void sn_rwlock_deinit(snRWLock *rw) {
    SN_UNUSED(rw);
    // Nothing to do
}

void sn_rwlock_rdlock(snRWLock *rw) {
    snRWLockWin32 *l = (snRWLockWin32 *)rw;
    AcquireSRWLockShared(&l->rwlock);
}

bool sn_rwlock_try_rdlock(snRWLock *rw) {
    snRWLockWin32 *l = (snRWLockWin32 *)rw;
    return TryAcquireSRWLockShared(&l->rwlock);
}

void sn_rwlock_rdunlock(snRWLock *rw) {
    snRWLockWin32 *l = (snRWLockWin32 *)rw;
    ReleaseSRWLockShared(&l->rwlock);
}

void sn_rwlock_wrlock(snRWLock *rw) {
    snRWLockWin32 *l = (snRWLockWin32 *)rw;
    AcquireSRWLockExclusive(&l->rwlock);
}

bool sn_rwlock_try_wrlock(snRWLock *rw) {
    snRWLockWin32 *l = (snRWLockWin32 *)rw;
    return TryAcquireSRWLockExclusive(&l->rwlock);
}

void sn_rwlock_wrunlock(snRWLock *rw) {
    snRWLockWin32 *l = (snRWLockWin32 *)rw;
    ReleaseSRWLockExclusive(&l->rwlock);
}

#endif

#include "snthreads/rwlock.h"

#if defined(SN_OS_WINDOWS)

#include <windows.h>

typedef struct snRWLockWin32 {
    SRWLOCK rwlock;
} snRWLockWin32;

#define RWLOCK(lock) (((snRWLockWin32 *)(lock))->rwlock)

SN_STATIC_ASSERT(sizeof(snRWLockWin32) <= sizeof(snRWLock), "snRWLock size is not large enough!");

bool sn_rwlock_init(snRWLock *rw) {
    InitializeSRWLock(&RWLOCK(rw));
    return true;
}

void sn_rwlock_deinit(snRWLock *rw) {
    SN_UNUSED(rw);
    // Nothing to do
}

void sn_rwlock_read_lock(snRWLock *rw) {
    AcquireSRWLockShared(&RWLOCK(rw));
}

bool sn_rwlock_try_read_lock(snRWLock *rw) {
    return TryAcquireSRWLockShared(&RWLOCK(rw));
}

void sn_rwlock_read_unlock(snRWLock *rw) {
    ReleaseSRWLockShared(&RWLOCK(rw));
}

void sn_rwlock_write_lock(snRWLock *rw) {
    AcquireSRWLockExclusive(&RWLOCK(rw));
}

bool sn_rwlock_try_write_lock(snRWLock *rw) {
    return TryAcquireSRWLockExclusive(&RWLOCK(rw));
}

void sn_rwlock_write_unlock(snRWLock *rw) {
    ReleaseSRWLockExclusive(&RWLOCK(rw));
}

#endif

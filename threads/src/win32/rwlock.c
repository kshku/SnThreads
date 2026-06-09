#include "snthreads/rwlock.h"

#if defined(SN_OS_WINDOWS)

    #include <windows.h>

typedef struct SnRWLockWin32 {
    SRWLOCK rwlock;
} SnRWLockWin32;

    #define RWLOCK(lock) (((SnRWLockWin32 *)(lock))->rwlock)

SN_STATIC_ASSERT(sizeof(SnRWLockWin32) <= sizeof(SnRWLock), "SnRWLock size is not large enough!");

bool sn_rwlock_init(SnRWLock *rw) {
    InitializeSRWLock(&RWLOCK(rw));
    return true;
}

void sn_rwlock_deinit(SnRWLock *rw) {
    SN_UNUSED(rw);
    // Nothing to do
}

void sn_rwlock_read_lock(SnRWLock *rw) {
    AcquireSRWLockShared(&RWLOCK(rw));
}

bool sn_rwlock_try_read_lock(SnRWLock *rw) {
    return TryAcquireSRWLockShared(&RWLOCK(rw));
}

void sn_rwlock_read_unlock(SnRWLock *rw) {
    ReleaseSRWLockShared(&RWLOCK(rw));
}

void sn_rwlock_write_lock(SnRWLock *rw) {
    AcquireSRWLockExclusive(&RWLOCK(rw));
}

bool sn_rwlock_try_write_lock(SnRWLock *rw) {
    return TryAcquireSRWLockExclusive(&RWLOCK(rw));
}

void sn_rwlock_write_unlock(SnRWLock *rw) {
    ReleaseSRWLockExclusive(&RWLOCK(rw));
}

#endif

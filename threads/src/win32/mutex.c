#include "snthreads/mutex.h"

#if defined(SN_OS_WINDOWS)

#include <windows.h>

typedef struct snMutexWin32 {
    CRITICAL_SECTION cs;
} snMutexWin32;

#define CS(mutex) (((snMutexWin32 *)(mutex))->cs)

SN_STATIC_ASSERT(sizeof(snMutexWin32) <= sizeof(snMutex), "snMutex size is not large enough!");

void sn_mutex_init(snMutex *m) {
    InitializeCriticalSection(&CS(m));
}

void sn_mutex_deinit(snMutex *m) {
    DeleteCriticalSection(&CS(m));
}

void sn_mutex_lock(snMutex *m) {
    EnterCriticalSection(&CS(m));
}

bool sn_mutex_try_lock(snMutex *m) {
    return TryEnterCriticalSection(&CS(m));
}

void sn_mutex_unlock(snMutex *m) {
    snMutexWin32 *mutex = (snMutexWin32 *)m;
    LeaveCriticalSection(&CS(m));
}

#endif

#include "snthreads/mutex.h"

#if defined(SN_OS_WINDOWS)

#include <windows.h>

typedef struct snMutexWin32 {
    CRITICAL_SECTION cs;
} snMutexWin32;

SN_STATIC_ASSERT(sizeof(snMutexWin32) <= sizeof(snMutex), "snMutex size is not large enough!");

void sn_mutex_init(snMutex *m) {
    snMutexWin32 *mutex = (snMutexWin32 *)m;
    InitializeCriticalSection(&mutex->cs);
}

void sn_mutex_deinit(snMutex *m) {
    snMutexWin32 *mutex = (snMutexWin32 *)m;
    DeleteCriticalSection(&mutex->cs);
}

void sn_mutex_lock(snMutex *m) {
    snMutexWin32 *mutex = (snMutexWin32 *)m;
    EnterCriticalSection(&mutex->mutex);
}

bool sn_mutex_try_lock(snMutex *m) {
    snMutexWin32 *mutex = (snMutexWin32 *)m;
    return TryEnterCriticalSection(&mutex->mutex);
}

void sn_mutex_unlock(snMutex *m) {
    snMutexWin32 *mutex = (snMutexWin32 *)m;
    LeaveCriticalSection(&mutex->mutex);
}

#endif

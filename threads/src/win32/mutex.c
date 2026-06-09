#include "snthreads/mutex.h"

#if defined(SN_OS_WINDOWS)

    #include <windows.h>

typedef struct SnMutexWin32 {
    CRITICAL_SECTION cs;
} SnMutexWin32;

    #define CS(mutex) (((SnMutexWin32 *)(mutex))->cs)

SN_STATIC_ASSERT(sizeof(SnMutexWin32) <= sizeof(SnMutex), "SnMutex size is not large enough!");

void sn_mutex_init(SnMutex *m) {
    InitializeCriticalSection(&CS(m));
}

void sn_mutex_deinit(SnMutex *m) {
    DeleteCriticalSection(&CS(m));
}

void sn_mutex_lock(SnMutex *m) {
    EnterCriticalSection(&CS(m));
}

bool sn_mutex_try_lock(SnMutex *m) {
    return TryEnterCriticalSection(&CS(m));
}

void sn_mutex_unlock(SnMutex *m) {
    SnMutexWin32 *mutex = (SnMutexWin32 *)m;
    LeaveCriticalSection(&CS(m));
}

#endif

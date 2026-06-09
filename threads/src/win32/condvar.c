#include "snthreads/condvar.h"

#if defined(SN_OS_WINDOWS)

    #include <windows.h>

typedef struct SnMutexWin32 {
    CRITICAL_SECTION cs;
} SnMutexWin32;

    #define CS(mutex) (((SnMutexWin32 *)(mutex))->cs)

typedef struct SnCondvarWin32 {
    CONDITION_VARIABLE cv;
} SnCondvarWin32;

    #define CV(condvar) (((SnCondvarWin32 *)(condvar))->cv)

SN_STATIC_ASSERT(sizeof(SnCondvarWin32) <= sizeof(SnCondvar), "SnCondvar size is not large "
                                                              "enough!");

bool sn_condvar_init(SnCondvar *cv) {
    InitializeConditionVariable(&CV(cv));
    return true;
}

void sn_condvar_deinit(SnCondvar *cv) {
    SN_UNUSED(cv);
    // Nothing to do
}

void sn_condvar_wait(SnCondvar *cv, SnMutex *mutex) {
    BOOL ok = SleepConditionVariableCS(&CV(cv), &CS(mutex), INFINITE);

    SN_ASSERT(ok && "SleepConditionVariableCS failed");
}

bool sn_condvar_timed_wait(SnCondvar *cv, SnMutex *mutex, uint64_t timeout_ns) {
    DWORD timeout_ms = (DWORD)(timeout_ns / 1000000ull);

    BOOL ok = SleepConditionVariableCS(&CV(cv), &CS(mutex), timeout_ms);

    if (ok) return true;

    DWORD err = GetLastError();
    return err != ERROR_TIMEOUT;
}

void sn_condvar_signal(SnCondvar *cv) {
    WakeConditionVariable(&CV(cv));
}

void sn_condvar_broadcast(SnCondvar *cv) {
    WakeAllConditionVariable(&CV(cv));
}

#endif


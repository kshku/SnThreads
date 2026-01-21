#include "snthreads/condvar.h"

#if defined(SN_OS_WINDOWS)

#include <windows.h>

typedef struct snMutexWin32 {
    CRITICAL_SECTION cs;
} snMutexWin32;

#define CS(mutex) (((snMutexWin32 *)(mutex))->cs)

typedef struct snCondvarWin32 {
    CONDITION_VARIABLE cv;
} snCondvarWin32;

#define CV(condvar) (((snCondvarWin32 *)(condvar))->cv)

SN_STATIC_ASSERT(sizeof(snCondvarWin32) <= sizeof(snCondvar), "snCondvar size is not large enough!");

bool sn_condvar_init(snCondvar *cv) {
    InitializeConditionVariable(&CV(cv));
    return true;
}

void sn_condvar_deinit(snCondvar *cv) {
    SN_UNUSED(cv);
    // Nothing to do
}

void sn_condvar_wait(snCondvar *cv, snMutex *mutex) {
    BOOL ok = SleepConditionVariableCS(&CV(cv), &CS(mutex), INFINITE);

    SN_ASSERT(ok && "SleepConditionVariableCS failed");
}

bool sn_condvar_timed_wait(snCondvar *cv, snMutex *mutex, uint64_t timeout_ns) {
    DWORD timeout_ms = (DWORD)(timeout_ns / 1000000ull);

    BOOL ok = SleepConditionVariableCS(&CV(cv), &CS(mutex), timeout_ms);

    if (ok) return true;

    DWORD err = GetLastError();
    return err != ERROR_TIMEOUT;
}

void sn_condvar_signal(snCondvar *cv) {
    WakeConditionVariable(&CV(cv));
}

void sn_condvar_broadcast(snCondvar *cv) {
    WakeAllConditionVariable(&CV(cv));
}

#endif


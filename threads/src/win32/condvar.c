#include "snthreads/condvar.h"

#if defined(SN_OS_WINDOWS)

#include <windows.h>

typedef struct snMutexWin32 {
    CRITICAL_SECTION cs;
} snMutexWin32;

typedef struct snCondvarWin32 {
    CONDITION_VARIABLE cv;
} snCondvarWin32;

SN_STATIC_ASSERT(sizeof(snCondvarWin32) <= sizeof(snCondvar), "snCondvar size is not large enough!");

bool sn_condvar_init(snCondvar *cv) {
    snCondvarWin32 *c = (snCondvarWin32 *)cv;
    InitializeConditionVariable(&c->cv);
    return true;
}

void sn_condvar_destroy(snCondvar *cv) {
    SN_UNUSED(cv);
    // Nothing to do
}

void sn_condvar_wait(snCondvar *cv, snMutex *mutex) {
    snCondvarWin32 *c = (snCondvarWin32 *)cv;
    snMutexWin32 *m = (snMutexWin32 *)mutex;

    BOOL ok = SleepConditionVariableCS(&c->cv, &m->cs, INFINITE);

    SN_ASSERT(ok && "SleepConditionVariableCS failed");
}

bool sn_condvar_timed_wait(snCondvar *cv, snMutex *mutex, uint64_t timeout_ns) {
    snCondvarWin32 *c = (snCondvarWin32 *)cv;
    snMutexWin32 *m = (snMutexWin32 *)mutex;

    DWORD timeout_ms = (DWORD)(timeout_ns / 1000000ull);

    BOOL ok = SleepConditionVariableCS(&c->cv, &m->cs, timeout_ms);

    if (ok) return true;

    DWORD err = GetLastError();
    return err != ERROR_TIMEOUT;
}

void sn_condvar_signal(snCondvar *cv) {
    snCondvarWin32 *c = (snCondvarWin32 *)cv;
    WakeConditionVariable(&c->cv);
}

void sn_condvar_broadcast(snCondvar *cv) {
    snCondvarWin32 *c = (snCondvarWin32 *)cv;
    WakeAllConditionVariable(&c->cv);
}

#endif


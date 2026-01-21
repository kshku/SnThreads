#include "snthreads/semaphore.h"

#if defined(SN_OS_WINDOWS)

#include <windows.h>

typedef struct snSemaphoreWin32 {
    HANDLE handle;
} snSemaphoreWin32;

#define HDL(semaphore) (((snSemaphoreWin32 *)(semaphore))->handle)

SN_STATIC_ASSERT(sizeof(snSemaphoreWin32) <= sizeof(snSemaphore), "snSemaphore size insufficient");

bool sn_semaphore_init(snSemaphore *sem, uint32_t initial_count) {
    snSemaphoreWin32 *s = (snSemaphoreWin32 *)sem;

    HDL(sem) = CreateSemaphoreW(NULL, (LONG)initial_count, LONG_MAX, NULL);

    return HDL(sem) != NULL;
}

void sn_semaphore_deinit(snSemaphore *sem) {
    snSemaphoreWin32 *s = (snSemaphoreWin32 *)sem;

    if (HDL(sem)) {
        CloseHandle(HDL(sem));
        HDL(sem) = NULL;
    }
}

void sn_semaphore_wait(snSemaphore *sem) {
    snSemaphoreWin32 *s = (snSemaphoreWin32 *)sem;

    DWORD r = WaitForSingleObject(HDL(sem), INFINITE);
    SN_ASSERT(r == WAIT_OBJECT_0);
}

bool sn_semaphore_try_wait(snSemaphore *sem) {
    snSemaphoreWin32 *s = (snSemaphoreWin32 *)sem;

    return WaitForSingleObject(HDL(sem), 0) == WAIT_OBJECT_0;
}

void sn_semaphore_post(snSemaphore *sem) {
    snSemaphoreWin32 *s = (snSemaphoreWin32 *)sem;

    BOOL ok = ReleaseSemaphore(HDL(sem), 1, NULL);
    SN_ASSERT(ok);
}

#endif


#include "snthreads/semaphore.h"

#if defined(SN_OS_WINDOWS)

    #include <windows.h>

typedef struct SnSemaphoreWin32 {
    HANDLE handle;
} SnSemaphoreWin32;

    #define HDL(semaphore) (((SnSemaphoreWin32 *)(semaphore))->handle)

SN_STATIC_ASSERT(sizeof(SnSemaphoreWin32) <= sizeof(SnSemaphore), "SnSemaphore size insufficient");

bool sn_semaphore_init(SnSemaphore *sem, uint32_t initial_count) {
    SnSemaphoreWin32 *s = (SnSemaphoreWin32 *)sem;

    HDL(sem) = CreateSemaphoreW(NULL, (LONG)initial_count, LONG_MAX, NULL);

    return HDL(sem) != NULL;
}

void sn_semaphore_deinit(SnSemaphore *sem) {
    SnSemaphoreWin32 *s = (SnSemaphoreWin32 *)sem;

    if (HDL(sem)) {
        CloseHandle(HDL(sem));
        HDL(sem) = NULL;
    }
}

void sn_semaphore_wait(SnSemaphore *sem) {
    SnSemaphoreWin32 *s = (SnSemaphoreWin32 *)sem;

    DWORD r = WaitForSingleObject(HDL(sem), INFINITE);
    SN_ASSERT(r == WAIT_OBJECT_0);
}

bool sn_semaphore_try_wait(SnSemaphore *sem) {
    SnSemaphoreWin32 *s = (SnSemaphoreWin32 *)sem;

    return WaitForSingleObject(HDL(sem), 0) == WAIT_OBJECT_0;
}

void sn_semaphore_post(SnSemaphore *sem) {
    SnSemaphoreWin32 *s = (SnSemaphoreWin32 *)sem;

    BOOL ok = ReleaseSemaphore(HDL(sem), 1, NULL);
    SN_ASSERT(ok);
}

#endif


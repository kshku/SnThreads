#include "snthreads/thread.h"

#if defined(SN_OS_WINDOWS)

    #include <windows.h>

    #ifdef SN_USE_FAST_TLS
_Thread_local SnThread *tls_fast = NULL;
    #endif

static SnThread main_thread;
static DWORD tls_os = TLS_OUT_OF_INDEXES;

static bool tls_init(void);
static void tls_shutdown(void);
static void tls_set(SnThread *t);
static SnThread *tls_get(void);
static void thread_detach_self(void);

static DWORD WINAPI sn_thread_wrapper(LPVOID arg);

typedef struct SnThreadWin32 {
    HANDLE handle;
    SnThreadFn fn;
    void *data;
    void *ret;
    DWORD tid;
    bool detached;
} SnThreadWin32;

SN_STATIC_ASSERT(sizeof(SnThreadWin32) <= sizeof(SnThread), "SnThread size is not large enough!");

bool sn_thread_init(void) {
    if (!tls_init()) return false;

    SnThreadWin32 *t = (SnThreadWin32 *)&main_thread;
    *t = (SnThreadWin32){.handle = GetCurrentThread(), .tid = GetCurrentThreadId(), .detached = true};

    tls_set(&main_thread);

    return true;
}

void sn_thread_shutdown(void) {
    #ifdef SN_USE_FAST_TLS
    tls_fast = NULL;
    #endif
    tls_shutdown();
}

bool sn_thread_create(SnThread *thread, SnThreadFn func, void *data) {
    SnThreadWin32 *t = (SnThreadWin32 *)thread;
    *t = (SnThreadWin32){.fn = func, .data = data, .detached = false};

    t->handle = CreateThread(NULL, 0, sn_thread_wrapper, t, 0, &t->tid);

    return t->handle != NULL;
}

bool sn_thread_detach(SnThread *thread) {
    SnThreadWin32 *t = (SnThreadWin32 *)thread;
    t->detached = true;

    if (t->handle) CloseHandle(t->handle);
    t->handle = NULL;

    return true;
}

bool sn_thread_join(SnThread *thread, void **ret) {
    SnThreadWin32 *t = (SnThreadWin32 *)thread;

    if (t->detached || !t->handle) return false;

    if (WaitForSingleObject(t->handle, INFINITE) != WAIT_OBJECT_0) return false;

    if (ret) *ret = t->ret;

    CloseHandle(t->handle);
    t->handle = NULL;

    return true;
}

void sn_thread_exit(void *ret) {
    SnThreadWin32 *t = (SnThreadWin32 *)tls_get();
    SN_ASSERT(t && "sn_thread_exit called without thread attached");

    if (t && !t->detached) t->ret = ret;

    thread_detach_self();
    ExitThread(0);
}

SnThread *sn_thread_self(void) {
    SnThread *t = tls_get();
    SN_ASSERT(t && "sn_thread_self called on unattached thread");
    return t;
}

bool sn_thread_equal(const SnThread *t1, const SnThread *t2) {
    if (t1 == t2) return true;

    return ((SnThreadWin32 *)t1)->tid == ((SnThreadWin32 *)t2)->tid;
}

bool sn_thread_attach(SnThread *thread) {
    SnThreadWin32 *t = (SnThreadWin32 *)thread;

    *t = (SnThreadWin32){.handle = GetCurrentThread(), .tid = GetCurrentThreadId(), .detached = true};

    tls_set(thread);
    return true;
}

static bool tls_init(void) {
    tls_os = TlsAlloc();
    return tls_os != TLS_OUT_OF_INDEXES;
}

static void tls_shutdown(void) {
    if (tls_os != TLS_OUT_OF_INDEXES) TlsFree(tls_os);

    tls_os = TLS_OUT_OF_INDEXES;
}

static void tls_set(SnThread *t) {
    #ifdef SN_USE_FAST_TLS
    tls_fast = t;
    #endif

    TlsSetValue(tls_os, t);
}

static SnThread *tls_get(void) {
    #ifdef SN_USE_FAST_TLS
    if (tls_fast) return tls_fast;
    #endif

    return (SnThread *)TlsGetValue(tls_os);
}

static DWORD WINAPI sn_thread_wrapper(LPVOID args) {
    SnThreadWin32 *thread = (SnThreadWin32 *)args;

    tls_set((SnThread *)thread);

    void *ret = thread->fn(thread->data);

    if (!thread->detached) thread->ret = ret;

    thread_detach_self();

    return 0;
}

static void thread_detach_self(void) {
    #ifdef SN_USE_FAST_TLS
    tls_fast = NULL;
    #endif
    TlsSetValue(tls_os, NULL);
}

#endif

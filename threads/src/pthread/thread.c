#include "snthreads/thread.h"

#if defined(SN_OS_LINUX) || defined(SN_OS_MAC)

    #include <pthread.h>

    #ifdef SN_USE_FAST_TLS
_Thread_local SnThread *tls_fast = NULL;
    #endif

static SnThread main_thread;
static pthread_key_t tls_os;

static bool tls_init(void);
static void tls_shutdown(void);
static void tls_set(SnThread *t);
static SnThread *tls_get(void);
static void thread_detach_self(void);

static void *sn_thread_wrapper(void *args);

typedef struct SnThreadPthread {
    SnThreadFn fn;
    void *data;
    pthread_t thread;
} SnThreadPthread;

    /**
     * @brief Get pthread_t from (SnThread *).
     */
    #define GET_PTHREAD(t) (((SnThreadPthread *)(t))->thread)

SN_STATIC_ASSERT(sizeof(SnThreadPthread) <= sizeof(SnThread), "SnThread size is not large enough!");

bool sn_thread_init(void) {
    if (!tls_init()) return false;

    SnThreadPthread *t = (SnThreadPthread *)&main_thread;
    *t = (SnThreadPthread){.thread = pthread_self()};

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
    SnThreadPthread *t = (SnThreadPthread *)thread;
    *t = (SnThreadPthread){.fn = func, .data = data};

    return pthread_create(&GET_PTHREAD(thread), NULL, sn_thread_wrapper, t) == 0;
}

bool sn_thread_detach(SnThread *thread) {
    return pthread_detach(GET_PTHREAD(thread)) == 0;
}

bool sn_thread_join(SnThread *thread, void **ret) {
    return pthread_join(GET_PTHREAD(thread), ret) == 0;
}

void sn_thread_exit(void *ret) {
    pthread_exit(ret);
}

SnThread *sn_thread_self(void) {
    SnThread *t = tls_get();
    SN_ASSERT(t && "sn_thread_self called on unattached thread");
    return t;
}

bool sn_thread_equal(const SnThread *t1, const SnThread *t2) {
    if (t1 == t2) return true;

    return pthread_equal(GET_PTHREAD(t1), GET_PTHREAD(t2));
}

bool sn_thread_attach(SnThread *thread) {
    SnThreadPthread *t = (SnThreadPthread *)thread;
    *t = (SnThreadPthread){.thread = pthread_self()};

    tls_set(thread);
    return true;
}

static bool tls_init(void) {
    return pthread_key_create(&tls_os, NULL) == 0;
}

static void tls_shutdown(void) {
    pthread_key_delete(tls_os);
}

static void tls_set(SnThread *t) {
    #ifdef SN_USE_FAST_TLS
    tls_fast = t;
    #endif

    pthread_setspecific(tls_os, t);
}

static SnThread *tls_get(void) {
    #ifdef SN_USE_FAST_TLS
    if (tls_fast) return tls_fast;
    #endif

    return (SnThread *)pthread_getspecific(tls_os);
}

static void *sn_thread_wrapper(void *args) {
    SnThreadPthread *thread = (SnThreadPthread *)args;

    tls_set((SnThread *)thread);

    void *ret = thread->fn(thread->data);

    thread_detach_self();

    return ret;
}

static void thread_detach_self(void) {
    #ifdef SN_USE_FAST_TLS
    tls_fast = NULL;
    #endif
    pthread_setspecific(tls_os, NULL);
}

#endif

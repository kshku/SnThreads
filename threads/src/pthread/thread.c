#include "snthreads/thread.h"

#if defined(SN_OS_LINUX) || defined(SN_OS_MAC)

#include <pthread.h>

#ifdef SN_USE_FAST_TLS
    _Thread_local snThread *tls_fast = NULL;
#endif

static snThread main_thread;
static pthread_key_t tls_os;

static bool tls_init(void);
static void tls_shutdown(void);
static void tls_set(snThread *t);
static snThread *tls_get(void);
static void thread_detach_self(void);

static void *sn_thread_wrapper(void *args);

typedef struct snThreadPthread {
    snThreadFn fn;
    void *data;
    pthread_t thread;
} snThreadPthread;

/**
 * @brief Get pthread_t from (snThread *).
 */
#define GET_PTHREAD(t) (((snThreadPthread *)(t))->thread)

SN_STATIC_ASSERT(sizeof(snThreadPthread) <= sizeof(snThread), "snThread size is not large enough!");

bool sn_thread_init(void) {
    if (!tls_init()) return false;

    snThreadPthread *t = (snThreadPthread *)&main_thread;
    *t = (snThreadPthread){
        .thread = pthread_self()
    };

    tls_set(&main_thread);

    return true;
}

void sn_thread_shutdown(void) {
#ifdef SN_USE_FAST_TLS
    tls_fast = NULL;
#endif
    tls_shutdown();
}

bool sn_thread_create(snThread *thread, snThreadFn func, void *data) {
    snThreadPthread *t = (snThreadPthread *)thread;
    *t = (snThreadPthread){
        .fn = func,
        .data = data
    };

    return pthread_create(&GET_PTHREAD(thread), NULL, sn_thread_wrapper, t) == 0;
}

bool sn_thread_detach(snThread *thread) {
    return pthread_detach(GET_PTHREAD(thread)) == 0;
}

bool sn_thread_join(snThread *thread, void **ret) {
    return pthread_join(GET_PTHREAD(thread), ret) == 0;
}

void sn_thread_exit(void *ret) {
    pthread_exit(ret);
}

snThread *sn_thread_self(void) {
    snThread *t = tls_get();
    SN_ASSERT(t && "sn_thread_self called on unattached thread");
    return t;
}

bool sn_thread_equal(const snThread *t1, const snThread *t2) {
    if (t1 == t2) return true;

    return pthread_equal(GET_PTHREAD(t1), GET_PTHREAD(t2));
}

bool sn_thread_attach(snThread *thread) {
    snThreadPthread *t = (snThreadPthread *)thread;
    *t = (snThreadPthread){
        .thread = pthread_self()
    };

    tls_set(thread);
    return true;
}

static bool tls_init(void) {
    return pthread_key_create(&tls_os, NULL) == 0;
}

static void tls_shutdown(void) {
    pthread_key_delete(tls_os);
}

static void tls_set(snThread *t) {
#ifdef SN_USE_FAST_TLS
    tls_fast = t;
#endif

    pthread_setspecific(tls_os, t);
}

static snThread *tls_get(void) {
#ifdef SN_USE_FAST_TLS
    if (tls_fast)
        return tls_fast;
#endif

    return (snThread *)pthread_getspecific(tls_os);
}

static void *sn_thread_wrapper(void *args) {
    snThreadPthread *thread = (snThreadPthread *)args;

    tls_set((snThread *)thread);

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

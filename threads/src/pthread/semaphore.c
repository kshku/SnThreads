#include "snthreads/semaphore.h"

#if defined(SN_OS_LINUX) || defined(SN_OS_MAC)

    #include "snthreads/condvar.h"
    #include "snthreads/mutex.h"

typedef struct SnSemaphorePthread {
    SnMutex mutex;
    SnCondvar cv;
    uint32_t count;
} SnSemaphorePthread;

SN_STATIC_ASSERT(sizeof(SnSemaphorePthread) <= sizeof(SnSemaphore), "SnSemaphore size "
                                                                    "insufficient");

bool sn_semaphore_init(SnSemaphore *sem, uint32_t initial_count) {
    SnSemaphorePthread *s = (SnSemaphorePthread *)sem;

    sn_mutex_init(&s->mutex);

    if (!sn_condvar_init(&s->cv)) {
        sn_mutex_deinit(&s->mutex);
        return false;
    }

    s->count = initial_count;
    return true;
}

void sn_semaphore_deinit(SnSemaphore *sem) {
    SnSemaphorePthread *s = (SnSemaphorePthread *)sem;

    sn_condvar_deinit(&s->cv);
    sn_mutex_deinit(&s->mutex);
}

void sn_semaphore_wait(SnSemaphore *sem) {
    SnSemaphorePthread *s = (SnSemaphorePthread *)sem;

    sn_mutex_lock(&s->mutex);

    while (s->count == 0) {
        sn_condvar_wait(&s->cv, &s->mutex);
    }

    s->count--;

    sn_mutex_unlock(&s->mutex);
}

bool sn_semaphore_try_wait(SnSemaphore *sem) {
    SnSemaphorePthread *s = (SnSemaphorePthread *)sem;

    bool acquired = false;

    sn_mutex_lock(&s->mutex);

    if (s->count > 0) {
        s->count--;
        acquired = true;
    }

    sn_mutex_unlock(&s->mutex);
    return acquired;
}

void sn_semaphore_post(SnSemaphore *sem) {
    SnSemaphorePthread *s = (SnSemaphorePthread *)sem;

    sn_mutex_lock(&s->mutex);

    s->count++;

    sn_condvar_signal(&s->cv);

    sn_mutex_unlock(&s->mutex);
}

#endif


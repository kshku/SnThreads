#include "snthreads/semaphore.h"

#if defined(SN_OS_LINUX) || defined(SN_OS_MAC)

#include "snthreads/mutex.h"
#include "snthreads/condvar.h"

typedef struct snSemaphorePthread {
    snMutex mutex;
    snCondvar cv;
    uint32_t count;
} snSemaphorePthread;

SN_STATIC_ASSERT(sizeof(snSemaphorePthread) <= sizeof(snSemaphore), "snSemaphore size insufficient");

bool sn_semaphore_init(snSemaphore *sem, uint32_t initial_count) {
    snSemaphorePthread *s = (snSemaphorePthread *)sem;

    sn_mutex_init(&s->mutex);

    if (!sn_condvar_init(&s->cv)) {
        sn_mutex_deinit(&s->mutex);
        return false;
    }

    s->count = initial_count;
    return true;
}

void sn_semaphore_deinit(snSemaphore *sem) {
    snSemaphorePthread *s = (snSemaphorePthread *)sem;

    sn_condvar_deinit(&s->cv);
    sn_mutex_deinit(&s->mutex);
}

void sn_semaphore_wait(snSemaphore *sem) {
    snSemaphorePthread *s = (snSemaphorePthread *)sem;

    sn_mutex_lock(&s->mutex);

    while (s->count == 0) {
        sn_condvar_wait(&s->cv, &s->mutex);
    }

    s->count--;

    sn_mutex_unlock(&s->mutex);
}

bool sn_semaphore_try_wait(snSemaphore *sem) {
    snSemaphorePthread *s = (snSemaphorePthread *)sem;

    bool acquired = false;

    sn_mutex_lock(&s->mutex);

    if (s->count > 0) {
        s->count--;
        acquired = true;
    }

    sn_mutex_unlock(&s->mutex);
    return acquired;
}

void sn_semaphore_post(snSemaphore *sem) {
    snSemaphorePthread *s = (snSemaphorePthread *)sem;

    sn_mutex_lock(&s->mutex);

    s->count++;

    sn_condvar_signal(&s->cv);

    sn_mutex_unlock(&s->mutex);
}

#endif


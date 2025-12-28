#include <snthreads/thread.h>
#include <snthreads/mutex.h>
#include <snthreads/condvar.h>
#include <snthreads/semaphore.h>
#include <snthreads/rwlock.h>

#include <stdio.h>
#include <stdlib.h>

#define TEST_ASSERT(x) \
    do { if (!(x)) { \
        fprintf(stderr, "FAIL: %s:%d: %s\n", __FILE__, __LINE__, #x); \
        abort(); \
    }} while (0)

#define TEST_PASS(name) \
    printf("[PASS] %s\n", name)

static void *thread_self_test(void *arg) {
    snThread *self = sn_thread_self();
    TEST_ASSERT(self != NULL);
    return arg;
}

void test_thread_self_basic(void) {
    snThread t;

    TEST_ASSERT(sn_thread_create(&t, thread_self_test, (void *)0x1234));

    void *ret = NULL;
    TEST_ASSERT(sn_thread_join(&t, &ret));
    TEST_ASSERT(ret == (void *)0x1234);

    TEST_PASS("thread_self_basic");
}

#define INC_THREADS 8
#define INC_ITERS   500000

static snMutex g_mutex;
static int g_counter;

static void *mutex_worker(void *arg) {
    (void)arg;
    for (int i = 0; i < INC_ITERS; ++i) {
        sn_mutex_lock(&g_mutex);
        g_counter++;
        sn_mutex_unlock(&g_mutex);
    }
    return NULL;
}

void test_mutex_contention(void) {
    snThread threads[INC_THREADS];

    sn_mutex_init(&g_mutex);
    g_counter = 0;

    for (int i = 0; i < INC_THREADS; ++i)
        TEST_ASSERT(sn_thread_create(&threads[i], mutex_worker, NULL));

    for (int i = 0; i < INC_THREADS; ++i)
        TEST_ASSERT(sn_thread_join(&threads[i], NULL));

    TEST_ASSERT(g_counter == INC_THREADS * INC_ITERS);

    sn_mutex_deinit(&g_mutex);

    TEST_PASS("mutex_contention");
}

#define RW_READERS 6
#define RW_WRITERS 2
#define RW_ITERS   100000

static snRWLock g_rwlock;
static int g_rw_value;

static void *reader(void *arg) {
    (void)arg;
    for (int i = 0; i < RW_ITERS; ++i) {
        sn_rwlock_read_lock(&g_rwlock);
        int v = g_rw_value;
        TEST_ASSERT(v >= 0);
        sn_rwlock_read_unlock(&g_rwlock);
    }
    return NULL;
}

static void *writer(void *arg) {
    (void)arg;
    for (int i = 0; i < RW_ITERS; ++i) {
        sn_rwlock_write_lock(&g_rwlock);
        g_rw_value++;
        sn_rwlock_write_unlock(&g_rwlock);
    }
    return NULL;
}

void test_rwlock(void) {
    snThread threads[RW_READERS + RW_WRITERS];

    TEST_ASSERT(sn_rwlock_init(&g_rwlock));
    g_rw_value = 0;

    for (int i = 0; i < RW_READERS; ++i)
        TEST_ASSERT(sn_thread_create(&threads[i], reader, NULL));

    for (int i = 0; i < RW_WRITERS; ++i)
        TEST_ASSERT(sn_thread_create(&threads[RW_READERS + i], writer, NULL));

    for (int i = 0; i < RW_READERS + RW_WRITERS; ++i)
        TEST_ASSERT(sn_thread_join(&threads[i], NULL));

    TEST_ASSERT(g_rw_value == RW_WRITERS * RW_ITERS);

    sn_rwlock_deinit(&g_rwlock);

    TEST_PASS("rwlock");
}

static snMutex cv_mutex;
static snCondvar cv;
static int cv_ready;

static void *cv_waiter(void *arg) {
    (void)arg;

    sn_mutex_lock(&cv_mutex);
    while (!cv_ready)
        sn_condvar_wait(&cv, &cv_mutex);
    sn_mutex_unlock(&cv_mutex);

    return NULL;
}

void test_condvar_wakeup(void) {
    snThread t;

    sn_mutex_init(&cv_mutex);
    TEST_ASSERT(sn_condvar_init(&cv));
    cv_ready = 0;

    TEST_ASSERT(sn_thread_create(&t, cv_waiter, NULL));

    sn_mutex_lock(&cv_mutex);
    cv_ready = 1;
    sn_condvar_signal(&cv);
    sn_mutex_unlock(&cv_mutex);

    TEST_ASSERT(sn_thread_join(&t, NULL));

    sn_condvar_deinit(&cv);
    sn_mutex_deinit(&cv_mutex);

    TEST_PASS("condvar_wakeup");
}

#define SEM_PRODUCERS 4
#define SEM_CONSUMERS 4
#define SEM_ITEMS     200000

static snSemaphore sem;
static snMutex sem_mutex;
static int produced;
static int consumed;

static void *producer(void *arg) {
    (void)arg;
    for (int i = 0; i < SEM_ITEMS; ++i) {
        sn_semaphore_post(&sem);

        sn_mutex_lock(&sem_mutex);
        produced++;
        sn_mutex_unlock(&sem_mutex);
    }
    return NULL;
}

static void *consumer(void *arg) {
    (void)arg;
    for (int i = 0; i < SEM_ITEMS; ++i) {
        sn_semaphore_wait(&sem);

        sn_mutex_lock(&sem_mutex);
        consumed++;
        sn_mutex_unlock(&sem_mutex);
    }
    return NULL;
}

void test_semaphore_pc(void) {
    snThread threads[SEM_PRODUCERS + SEM_CONSUMERS];

    TEST_ASSERT(sn_semaphore_init(&sem, 0));
    sn_mutex_init(&sem_mutex);

    produced = consumed = 0;

    for (int i = 0; i < SEM_PRODUCERS; ++i)
        TEST_ASSERT(sn_thread_create(&threads[i], producer, NULL));

    for (int i = 0; i < SEM_CONSUMERS; ++i)
        TEST_ASSERT(sn_thread_create(&threads[SEM_PRODUCERS + i], consumer, NULL));

    for (int i = 0; i < SEM_PRODUCERS + SEM_CONSUMERS; ++i)
        TEST_ASSERT(sn_thread_join(&threads[i], NULL));

    TEST_ASSERT(produced == consumed);
    TEST_ASSERT(produced == SEM_PRODUCERS * SEM_ITEMS);

    sn_mutex_deinit(&sem_mutex);
    sn_semaphore_deinit(&sem);

    TEST_PASS("semaphore_pc");
}

void test_thread_self_without_attach_should_assert(void) {
    SN_UNUSED(sn_thread_self());
}

int main(void) {
    // test_thread_self_without_attach_should_assert();
    TEST_ASSERT(sn_thread_init());

    test_thread_self_basic();
    test_mutex_contention();
    test_rwlock();
    test_condvar_wakeup();
    test_semaphore_pc();

    sn_thread_shutdown();

    printf("ALL TESTS PASSED\n");
    return 0;
}


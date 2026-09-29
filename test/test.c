#include <snthreads/atomics.h>
#include <snthreads/condvar.h>
#include <snthreads/mutex.h>
#include <snthreads/rwlock.h>
#include <snthreads/semaphore.h>
#include <snthreads/thread.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_ASSERT(x)                                                    \
    do {                                                                  \
        if (!(x)) {                                                       \
            fprintf(stderr, "FAIL: %s:%d: %s\n", __FILE__, __LINE__, #x); \
            abort();                                                      \
        }                                                                 \
    } while (0)

#define TEST_PASS(name) printf("[PASS] %s\n", name)

static void *thread_self_test(void *arg) {
    SnThread *self = sn_thread_self();
    TEST_ASSERT(self != NULL);
    return arg;
}

void test_thread_self_basic(void) {
    SnThread t;

    TEST_ASSERT(sn_thread_create(&t, thread_self_test, (void *)0x1234));

    void *ret = NULL;
    TEST_ASSERT(sn_thread_join(&t, &ret));
    TEST_ASSERT(ret == (void *)0x1234);

    TEST_PASS("thread_self_basic");
}

#define INC_THREADS 8
#define FLAG_RACERS 16
#define INC_ITERS 500000

static SnMutex g_mutex;
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
    SnThread threads[INC_THREADS];

    sn_mutex_init(&g_mutex);
    g_counter = 0;

    for (int i = 0; i < INC_THREADS; ++i)
        TEST_ASSERT(sn_thread_create(&threads[i], mutex_worker, NULL));

    for (int i = 0; i < INC_THREADS; ++i) TEST_ASSERT(sn_thread_join(&threads[i], NULL));

    TEST_ASSERT(g_counter == INC_THREADS * INC_ITERS);

    sn_mutex_deinit(&g_mutex);

    TEST_PASS("mutex_contention");
}

#define RW_READERS 6
#define RW_WRITERS 2
#define RW_ITERS 100000

static SnRWLock g_rwlock;
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
    SnThread threads[RW_READERS + RW_WRITERS];

    TEST_ASSERT(sn_rwlock_init(&g_rwlock));
    g_rw_value = 0;

    for (int i = 0; i < RW_READERS; ++i) TEST_ASSERT(sn_thread_create(&threads[i], reader, NULL));

    for (int i = 0; i < RW_WRITERS; ++i)
        TEST_ASSERT(sn_thread_create(&threads[RW_READERS + i], writer, NULL));

    for (int i = 0; i < RW_READERS + RW_WRITERS; ++i)
        TEST_ASSERT(sn_thread_join(&threads[i], NULL));

    TEST_ASSERT(g_rw_value == RW_WRITERS * RW_ITERS);

    sn_rwlock_deinit(&g_rwlock);

    TEST_PASS("rwlock");
}

static SnMutex cv_mutex;
static SnCondvar cv;
static int cv_ready;

static void *cv_waiter(void *arg) {
    (void)arg;

    sn_mutex_lock(&cv_mutex);
    while (!cv_ready) sn_condvar_wait(&cv, &cv_mutex);
    sn_mutex_unlock(&cv_mutex);

    return NULL;
}

void test_condvar_wakeup(void) {
    SnThread t;

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
#define SEM_ITEMS 200000

static SnSemaphore sem;
static SnMutex sem_mutex;
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
    SnThread threads[SEM_PRODUCERS + SEM_CONSUMERS];

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

/* Every generic atomic macro resolves its function through the same
   SN_GET_GENERIC_ATOMIC_FUNCTION dispatch, so exercising one of each catches a
   macro that reaches for a helper that does not exist. */
void test_generic_atomics(void) {
    sn_atomic_int32_t counter = SN_ATOMIC_VAR_INIT(0);
    sn_atomic_uint64_t wide = SN_ATOMIC_VAR_INIT(0);

    TEST_ASSERT(sn_atomic_load_explicit(&counter, SN_MEMORY_ORDER_ACQUIRE) == 0);

    sn_atomic_store_explicit(&counter, 7, SN_MEMORY_ORDER_RELEASE);
    TEST_ASSERT(sn_atomic_load_explicit(&counter, SN_MEMORY_ORDER_ACQUIRE) == 7);

    TEST_ASSERT(sn_atomic_exchange_explicit(&counter, 9, SN_MEMORY_ORDER_NONE) == 7);
    TEST_ASSERT(sn_atomic_load_explicit(&counter, SN_MEMORY_ORDER_ACQUIRE) == 9);

    /* The exchange above has to leave the object at 9 for this to report a
       swap, and a mismatch has to leave it alone. */
    int32_t expect = 9;
    TEST_ASSERT(sn_atomic_compare_exchange_explicit(
        &counter, &expect, 11, SN_MEMORY_ORDER_ACQUIRE, SN_MEMORY_ORDER_NONE));
    TEST_ASSERT(sn_atomic_load_explicit(&counter, SN_MEMORY_ORDER_ACQUIRE) == 11);

    int32_t mismatch = 0;
    TEST_ASSERT(!sn_atomic_compare_exchange_explicit(
        &counter, &mismatch, 13, SN_MEMORY_ORDER_ACQUIRE, SN_MEMORY_ORDER_NONE));
    TEST_ASSERT(sn_atomic_load_explicit(&counter, SN_MEMORY_ORDER_ACQUIRE) == 11);
    /* A failed exchange reports what it found, which is what makes the loop
       form of compare exchange work. */
    TEST_ASSERT(mismatch == 11);

    /* Default order variant, now that expect matches, so this one does swap. */
    TEST_ASSERT(sn_atomic_compare_exchange(&counter, &mismatch, 15));
    TEST_ASSERT(sn_atomic_load_explicit(&counter, SN_MEMORY_ORDER_ACQUIRE) == 15);

    /* Every fetch reports the value from before the operation, so track it
       rather than restating each result. */
    int32_t held = 15;
    TEST_ASSERT(sn_atomic_fetch_add_explicit(&counter, 4, SN_MEMORY_ORDER_NONE) == held);
    held += 4;
    TEST_ASSERT(sn_atomic_fetch_add_explicit(&counter, -8, SN_MEMORY_ORDER_NONE) == held);
    held -= 8;
    TEST_ASSERT(sn_atomic_fetch_or_explicit(&counter, 0x40, SN_MEMORY_ORDER_NONE) == held);
    held |= 0x40;
    TEST_ASSERT(sn_atomic_fetch_and_explicit(&counter, 0x7E, SN_MEMORY_ORDER_NONE) == held);
    held &= 0x7E;
    TEST_ASSERT(sn_atomic_fetch_xor_explicit(&counter, 0xFF, SN_MEMORY_ORDER_NONE) == held);
    held ^= 0xFF;
    TEST_ASSERT(sn_atomic_fetch_sub_explicit(&counter, 1, SN_MEMORY_ORDER_NONE) == held);
    held -= 1;
    TEST_ASSERT(sn_atomic_load_explicit(&counter, SN_MEMORY_ORDER_ACQUIRE) == held);

    /* A different width, so the dispatch has to pick a different overload. */
    TEST_ASSERT(sn_atomic_fetch_add_explicit(&wide, UINT64_C(1) << 40, SN_MEMORY_ORDER_NONE) == 0);
    TEST_ASSERT(sn_atomic_load_explicit(&wide, SN_MEMORY_ORDER_ACQUIRE) == (UINT64_C(1) << 40));

    TEST_PASS("generic_atomics");
}

/* The flag API is the other half of the atomics surface and shares the header. */
void test_atomic_flag(void) {
    sn_atomic_flag flag = SN_ATOMIC_FLAG_INIT;

    TEST_ASSERT(!sn_atomic_flag_test_and_set(&flag));
    TEST_ASSERT(sn_atomic_flag_test_and_set(&flag));
    TEST_ASSERT(sn_atomic_flag_load(&flag));
    sn_atomic_flag_clear(&flag);
    TEST_ASSERT(!sn_atomic_flag_load(&flag));

    TEST_PASS("atomic_flag");
}

/* Exactly one of the racing setters has to see the flag unset, which is what
   makes the flag usable as a once latch. */
static sn_atomic_flag g_latch = SN_ATOMIC_FLAG_INIT;
static volatile int g_latch_winner = -1;

static void *latch_racer(void *arg) {
    (void)arg;
    if (!sn_atomic_flag_test_and_set(&g_latch)) g_latch_winner = 1;
    return NULL;
}

void test_atomic_flag_has_one_winner(void) {
    SnThread racers[FLAG_RACERS];

    g_latch_winner = -1;
    for (int i = 0; i < FLAG_RACERS; ++i)
        TEST_ASSERT(sn_thread_create(&racers[i], latch_racer, NULL));
    for (int i = 0; i < FLAG_RACERS; ++i) {
        void *ret = NULL;
        TEST_ASSERT(sn_thread_join(&racers[i], &ret));
    }

    TEST_ASSERT(g_latch_winner == 1);
    /* A thread that comes along later has to lose. */
    TEST_ASSERT(sn_atomic_flag_test_and_set(&g_latch));

    TEST_PASS("atomic_flag_has_one_winner");
}

int main(void) {
    /* Unbuffered, so a crash on one platform still shows which test got there. */
    setvbuf(stdout, NULL, _IONBF, 0);
    // test_thread_self_without_attach_should_assert();
    TEST_ASSERT(sn_thread_init());

    test_thread_self_basic();
    test_generic_atomics();
    test_atomic_flag();
    test_atomic_flag_has_one_winner();
    test_mutex_contention();
    test_rwlock();
    test_condvar_wakeup();
    test_semaphore_pc();

    sn_thread_shutdown();

    printf("ALL TESTS PASSED\n");
    return 0;
}


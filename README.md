# SnThreads

Thread abstraction and synchronization library written in C17.

Provides threads, mutexes, condition variables, read-write locks, counting semaphores,
spinlocks, and atomic operations with a cross-platform API.

## API

### Threads (`<snthreads/thread.h>`)

| Function | Description |
|----------|-------------|
| `bool sn_thread_init(void)` | Initialize threading subsystem (call once before any other threading function) |
| `void sn_thread_shutdown(void)` | Shutdown threading subsystem |
| `bool sn_thread_create(SnThread *thread, SnThreadFn func, void *data)` | Create a new thread |
| `bool sn_thread_join(SnThread *thread, void **ret)` | Join a thread, block until termination |
| `bool sn_thread_detach(SnThread *thread)` | Detach a thread (resources auto-released on exit) |
| `SnThread *sn_thread_self(void)` | Get the current thread's SnThread pointer |
| `bool sn_thread_equal(const SnThread *t1, const SnThread *t2)` | Compare two thread IDs |
| `bool sn_thread_attach(SnThread *thread)` | Attach an external thread (e.g., main thread) to the threading system |

### Mutex (`<snthreads/mutex.h>`)

| Function | Description |
|----------|-------------|
| `void sn_mutex_init(SnMutex *m)` | Initialize a mutex (not recursive) |
| `void sn_mutex_deinit(SnMutex *m)` | Destroy a mutex |
| `void sn_mutex_lock(SnMutex *m)` | Lock, blocks if contended |
| `bool sn_mutex_try_lock(SnMutex *m)` | Try to lock, returns true if acquired |
| `void sn_mutex_unlock(SnMutex *m)` | Unlock |

### Condition Variable (`<snthreads/condvar.h>`)

| Function | Description |
|----------|-------------|
| `bool sn_condvar_init(SnCondvar *cv)` | Initialize a condition variable |
| `void sn_condvar_deinit(SnCondvar *cv)` | Destroy a condition variable |
| `void sn_condvar_wait(SnCondvar *cv, SnMutex *mutex)` | Wait (mutex must be locked; released while waiting) |
| `bool sn_condvar_timed_wait(SnCondvar *cv, SnMutex *mutex, uint64_t timeout_ns)` | Timed wait (absolute timeout in ns) |
| `void sn_condvar_signal(SnCondvar *cv)` | Wake one waiter |
| `void sn_condvar_broadcast(SnCondvar *cv)` | Wake all waiters |

### Read-Write Lock (`<snthreads/rwlock.h>`)

| Function | Description |
|----------|-------------|
| `bool sn_rwlock_init(SnRWLock *rw)` | Initialize |
| `void sn_rwlock_deinit(SnRWLock *rw)` | Destroy |
| `void sn_rwlock_read_lock(SnRWLock *rw)` | Acquire shared (read) lock |
| `bool sn_rwlock_try_read_lock(SnRWLock *rw)` | Try read lock |
| `void sn_rwlock_read_unlock(SnRWLock *rw)` | Release read lock |
| `void sn_rwlock_write_lock(SnRWLock *rw)` | Acquire exclusive (write) lock |
| `bool sn_rwlock_try_write_lock(SnRWLock *rw)` | Try write lock |
| `void sn_rwlock_write_unlock(SnRWLock *rw)` | Release write lock |

### Semaphore (`<snthreads/semaphore.h>`)

| Function | Description |
|----------|-------------|
| `bool sn_semaphore_init(SnSemaphore *sem, uint32_t initial_count)` | Initialize with starting count |
| `void sn_semaphore_deinit(SnSemaphore *sem)` | Destroy (no threads may be waiting) |
| `void sn_semaphore_wait(SnSemaphore *sem)` | Decrement, block if zero |
| `bool sn_semaphore_try_wait(SnSemaphore *sem)` | Try to decrement without blocking |
| `void sn_semaphore_post(SnSemaphore *sem)` | Increment and wake one waiter |

### Spinlock (`<snthreads/spinlock.h>`)

```c
typedef sn_atomic_flag sn_spinlock;
#define SN_SPINLOCK_INIT SN_ATOMIC_FLAG_INIT
#define sn_spinlock_lock(lock)      // spin until acquired (with pause)
#define sn_spinlock_try_lock(lock)  // returns true if acquired
#define sn_spinlock_unlock(lock)    // release
```

### Atomics (`<snthreads/atomics.h>`)

Atomic load, store, exchange, CAS, fetch-add/sub/or/xor/and for all standard integer types,
plus memory fences and atomic flag operations (test-and-set, clear, load).

## Usage

```c
#include <snthreads/thread.h>
#include <snthreads/mutex.h>
#include <stdio.h>

static SnMutex mutex;
static int counter;

static void *worker(void *arg) {
    (void)arg;
    for (int i = 0; i < 100000; ++i) {
        sn_mutex_lock(&mutex);
        counter++;
        sn_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(void) {
    sn_thread_init();
    sn_mutex_init(&mutex);

    SnThread threads[8];
    for (int i = 0; i < 8; ++i)
        sn_thread_create(&threads[i], worker, NULL);
    for (int i = 0; i < 8; ++i)
        sn_thread_join(&threads[i], NULL);

    printf("Counter: %d\n", counter);  // 800000

    sn_mutex_deinit(&mutex);
    sn_thread_shutdown();
    return 0;
}
```

## Adding to your project

```cmake
include(FetchContent)
FetchContent_Declare(snthreads
    GIT_REPOSITORY https://github.com/kshku/SnThreads.git
    GIT_TAG <tag>  # e.g., v0.1.0
)
FetchContent_MakeAvailable(snthreads)

target_link_libraries(myapp PRIVATE snthreads)
```

## Build

```sh
cmake -B build
cmake --build build
```

| Option | Default | Description |
|--------|---------|-------------|
| `SN_THREADS_BUILD_SHARED` | `OFF` | Build as shared library |
| `SN_THREADS_BUILD_TEST` | `OFF` | Build tests |

## Platform Support

| Platform | Backend |
|----------|---------|
| Linux | pthreads |
| macOS | pthreads |
| Windows | Win32 threads, CRITICAL_SECTION, SRWLOCK, CONDITION_VARIABLE |

## Dependencies

- **SnCore** — fetched automatically via FetchContent

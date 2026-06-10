#pragma once

#include <sncore/defines.h>
#include "snthreads/mutex.h"

// Should be included after mutex.h
#include "snthreads/api.h"

#if !defined(SN_DISABLE_FAST_TLS)
    #define SN_USE_FAST_TLS
#endif

/**
 * @typedef SnCondvar
 * @brief Condition variable type.
 *
 * Ownership & lifetime rules:
 * - SnCondvar is user-owned storage.
 * - Must be initialized before use.
 * - Must outlive all waiting threads.
 *
 * Semantics:
 * - Wait must be called with mutex locked.
 * - Mutex is atomically released while waiting.
 * - Mutex is re-acquired before returning.
 * - Spurious wakeups are allowed.
 */
typedef struct SnCondvar {
    alignas(max_align_t) char buffer[64];
} SnCondvar;

/**
 * @brief Initialize a condition variable.
 *
 * @param cv Pointer to condvar.
 *
 * @return Returns true on success.
 */
SN_API bool sn_condvar_init(SnCondvar *cv);

/**
 * @brief Deinitialize a condition variable.
 *
 * @param cv Pointer to condvar.
 *
 * No threads may be waiting.
 */
SN_API void sn_condvar_deinit(SnCondvar *cv);

/**
 * @brief Wait on condition variable.
 *
 * @param cv Pointer to condvar.
 * @param mutex The mutex.
 *
 * Mutex must be locked before calling.
 * Returns with mutex locked.
 */
SN_API void sn_condvar_wait(SnCondvar *cv, SnMutex *mutex);

/**
 * @brief Timed wait on condition variable.
 *
 * @param cv Pointer to condvar.
 * @param mutex The mutex.
 * @param timeout_ns Absolute timeout in nanoseconds.
 *
 * @return true if signaled, false if timed out.
 */
SN_API bool sn_condvar_timed_wait(SnCondvar *cv, SnMutex *mutex, uint64_t timeout_ns);

/**
 * @brief Wake one waiting thread.
 *
 * @param cv Pointer to condvar.
 */
SN_API void sn_condvar_signal(SnCondvar *cv);

/**
 * @brief Wake all waiting threads.
 *
 * @param cv Pointer to condvar.
 */
SN_API void sn_condvar_broadcast(SnCondvar *cv);

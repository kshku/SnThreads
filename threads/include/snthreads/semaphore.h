#pragma once

#include "snthreads/api.h"
#include "snthreads/defines.h"

/**
 * @typedef SnSemaphore
 * @brief Counting semaphore.
 *
 * Ownership & lifetime:
 * - User-owned storage
 * - Must be initialized before use
 * - Must outlive all waiters
 *
 * Semantics:
 * - Count is >= 0
 * - wait(): decrements count or blocks
 * - post(): increments count and wakes waiter
 */
typedef struct SnSemaphore {
    alignas(max_align_t) char buffer[256];
} SnSemaphore;

/**
 * @brief Initialize semaphore with initial count.
 */
SN_API bool sn_semaphore_init(SnSemaphore *sem, uint32_t initial_count);

/**
 * @brief Deinitialize semaphore.
 *
 * No threads may be waiting.
 */
SN_API void sn_semaphore_deinit(SnSemaphore *sem);

/**
 * @brief Decrement semaphore or block.
 */
SN_API void sn_semaphore_wait(SnSemaphore *sem);

/**
 * @brief Try to decrement semaphore without blocking.
 *
 * @return true on success, false if count was zero.
 */
SN_API bool sn_semaphore_try_wait(SnSemaphore *sem);

/**
 * @brief Increment semaphore and wake one waiter.
 */
SN_API void sn_semaphore_post(SnSemaphore *sem);

#undef SN_API

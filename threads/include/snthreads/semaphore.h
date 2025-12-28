#pragma once

#include "snthreads/defines.h"

/**
 * @typedef snSemaphore
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
typedef struct snSemaphore {
    alignas(max_align_t) char buffer[256];
} snSemaphore;

/**
 * @brief Initialize semaphore with initial count.
 */
SN_API bool sn_semaphore_init(snSemaphore *sem, uint32_t initial_count);

/**
 * @brief Deinitialize semaphore.
 *
 * No threads may be waiting.
 */
SN_API void sn_semaphore_deinit(snSemaphore *sem);

/**
 * @brief Decrement semaphore or block.
 */
SN_API void sn_semaphore_wait(snSemaphore *sem);

/**
 * @brief Try to decrement semaphore without blocking.
 *
 * @return true on success, false if count was zero.
 */
SN_API bool sn_semaphore_try_wait(snSemaphore *sem);

/**
 * @brief Increment semaphore and wake one waiter.
 */
SN_API void sn_semaphore_post(snSemaphore *sem);


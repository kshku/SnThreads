#pragma once

#include "snthreads/api.h"
#include <sncore/defines.h>

/**
 * @struct SnRWLock
 * @brief SnRWLock (read write lock) type.
 */
typedef struct SnRWLock {
    alignas(16) char buffer[80];
} SnRWLock;

/**
 * @brief Initialize read write lock.
 *
 * @param rw Pointer to rwlock.
 *
 * @return Returns true on success, else false.
 */
SN_THREADS_API bool sn_rwlock_init(SnRWLock *rw);

/**
 * @brief Deinitialize read write lock.
 *
 * @param rw Pointer to rwlock.
 */
SN_THREADS_API void sn_rwlock_deinit(SnRWLock *rw);

/**
 * Acquire read lock.
 *
 * @param rw Pointer to rwlock.
 */
SN_THREADS_API void sn_rwlock_read_lock(SnRWLock *rw);

/**
 * @brief Try to acquire read lock.
 *
 * @param rw Pointer to rwlock.
 *
 * @return Returns true if lock is acquired.
 */
SN_THREADS_API bool sn_rwlock_try_read_lock(SnRWLock *rw);

/**
 * @brief Release read lock.
 *
 * @param rw Pointer to rwlock.
 */
SN_THREADS_API void sn_rwlock_read_unlock(SnRWLock *rw);

/**
 * Acquire write lock.
 *
 * @param rw Pointer to rwlock.
 */
SN_THREADS_API void sn_rwlock_write_lock(SnRWLock *rw);

/**
 * @brief Try to acquire write lock.
 *
 * @param rw Pointer to rwlock.
 *
 * @return Returns true if lock is acquired.
 */
SN_THREADS_API bool sn_rwlock_try_write_lock(SnRWLock *rw);

/**
 * @brief Release write lock.
 *
 * @param rw Pointer to rwlock.
 */
SN_THREADS_API void sn_rwlock_write_unlock(SnRWLock *rw);


#pragma once

#include "snthreads/defines.h"

/**
 * @struct snRWLock
 * @brief snRWLock (read write lock) type.
 */
typedef struct snRWLock {
    alignas(max_align_t) char buffer[80];
} snRWLock;


/**
 * @brief Initialize read write lock.
 *
 * @param rw Pointer to rwlock.
 *
 * @return Returns true on success, else false.
 */
SN_API bool sn_rwlock_init(snRWLock *rw);

/**
 * @brief Deinitialize read write lock.
 *
 * @param rw Pointer to rwlock.
 */
SN_API void sn_rwlock_deinit(snRWLock *rw);

/**
 * Acquire read lock.
 *
 * @param rw Pointer to rwlock.
 */
SN_API void sn_rwlock_read_lock(snRWLock *rw);

/**
 * @brief Try to acquire read lock.
 *
 * @param rw Pointer to rwlock.
 *
 * @return Returns true if lock is acquired.
 */
SN_API bool sn_rwlock_try_read_lock(snRWLock *rw);

/**
 * @brief Release read lock.
 *
 * @param rw Pointer to rwlock.
 */
SN_API void sn_rwlock_read_unlock(snRWLock *rw);

/**
 * Acquire write lock.
 *
 * @param rw Pointer to rwlock.
 */
SN_API void sn_rwlock_write_lock(snRWLock *rw);

/**
 * @brief Try to acquire write lock.
 *
 * @param rw Pointer to rwlock.
 *
 * @return Returns true if lock is acquired.
 */
SN_API bool sn_rwlock_try_write_lock(snRWLock *rw);

/**
 * @brief Release write lock.
 *
 * @param rw Pointer to rwlock.
 */
SN_API void sn_rwlock_write_unlock(snRWLock *rw);


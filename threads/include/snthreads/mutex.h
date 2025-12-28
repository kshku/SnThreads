#pragma once

#include "snthreads/defines.h"

/**
 * @struct snMutex
 * @brief snMutex type.
 *
 * @note Not recursive!
 */
typedef struct snMutex {
    alignas(max_align_t) char buffer[64];
} snMutex;

/**
 * @brief Initialize the mutex.
 *
 * @param m Pointer to mutex.
 */
SN_API void sn_mutex_init(snMutex *m);

/**
 * @brief Deinitialize the mutex.
 *
 * @param m Pointer to mutex.
 */
SN_API void sn_mutex_deinit(snMutex *m);

/**
 * @brief Acquire mutex lock.
 *
 * @param m Pointer to mutex.
 */
SN_API void sn_mutex_lock(snMutex *m);

/**
 * @brief Try to aquire mutex lock.
 *
 * @param m Pointer to mutex.
 *
 * @return Returns true if lock is acquired.
 */
SN_API bool sn_mutex_try_lock(snMutex *m);

/**
 * @brief Release the mutex lock.
 *
 * @param m Pointer to the mutex.
 */
SN_API void sn_mutex_unlock(snMutex *m);

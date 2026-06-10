#pragma once

#include "snthreads/api.h"
#include <sncore/defines.h>

/**
 * @struct SnMutex
 * @brief SnMutex type.
 *
 * @note Not recursive!
 */
typedef struct SnMutex {
    alignas(max_align_t) char buffer[64];
} SnMutex;

/**
 * @brief Initialize the mutex.
 *
 * @param m Pointer to mutex.
 */
SN_API void sn_mutex_init(SnMutex *m);

/**
 * @brief Deinitialize the mutex.
 *
 * @param m Pointer to mutex.
 */
SN_API void sn_mutex_deinit(SnMutex *m);

/**
 * @brief Acquire mutex lock.
 *
 * @param m Pointer to mutex.
 */
SN_API void sn_mutex_lock(SnMutex *m);

/**
 * @brief Try to aquire mutex lock.
 *
 * @param m Pointer to mutex.
 *
 * @return Returns true if lock is acquired.
 */
SN_API bool sn_mutex_try_lock(SnMutex *m);

/**
 * @brief Release the mutex lock.
 *
 * @param m Pointer to the mutex.
 */
SN_API void sn_mutex_unlock(SnMutex *m);

#undef SN_API

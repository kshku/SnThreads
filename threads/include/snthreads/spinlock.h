#pragma once

#include "snthreads/defines.h"
#include "snthreads/atomics.h"

#include "snthreads/api.h"

#if defined(SN_COMPILER_MSVC)
    SN_API void sn_pause_instruction(void);
    #define SN_PAUSE_INSTRUCTION sn_pause_instruction()
#else
    #define SN_PAUSE_INSTRUCTION __asm__ volatile("pause")
#endif

/**
 * @brief The spinlock type
 */
typedef sn_atomic_flag sn_spinlock;

/**
 * @brief Initialize the spinlock.
 */
#define SN_SPINLOCK_INIT SN_ATOMIC_FLAG_INIT

/**
 * @brief Get the lock.
 */
#define sn_spinlock_lock(lock)                                               \
    while (                                                                  \
        sn_atomic_flag_test_and_set_explicit(lock, SN_MEMORY_ORDER_ACQUIRE)) \
        SN_PAUSE_INSTRUCTION;

/**
 * @brief Try to get the lock.
 *
 * @return Returns true if lock is acquired, else false.
 */
#define sn_spinlock_try_lock(lock) \
    !sn_atomic_flag_test_and_set_explicit(lock, SN_MEMORY_ORDER_ACQUIRE)

/**
 * @brief Release the lock.
 */
#define sn_spinlock_unlock(lock) \
    sn_atomic_flag_clear_explicit(lock, SN_MEMORY_ORDER_RELEASE)

/**
 * @brief Check whether locked (doesn't perform lock or unlock).
 *
 * @return Returns true if locked, else false.
 */
#define sn_spinlock_locked(lock) \
    sn_atomic_flag_load_explicit(lock, SN_MEMORY_ORDER_ACQUIRE)

#undef SN_API

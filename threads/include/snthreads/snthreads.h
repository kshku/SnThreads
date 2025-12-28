#pragma once

#include "snthreads/defines.h"

#if !defined(SN_DISABLE_FAST_TLS)
    #define SN_USE_FAST_TLS
#endif

/**
 * @typedef snThread
 * @brief The thread type.
 *
 * Defined according to platform.
 *
 * Ownership & lifetime rules:
 * - snThread is user-owned storage.
 * - The snThread object must remain valid until the thread has exited,
 *   regardless of whether it is joined or detached.
 * - Detached threads cannot be joined.
 * - Calling sn_thread_self() is only valid while the thread is attached.
 */
typedef struct snThread {
    alignas(max_align_t) char buffer[40];
} snThread;

/**
 * typedef snThreadFn
 * @brief The type of function called after creating thread.
 *
 * @param data The data passed while creating thread.
 *
 * @return Returns a void *.
 */
typedef void *(*snThreadFn)(void *data);

/**
 * @brief Initialize the threading related things.
 *
 * Must be called once before using any other threading functions.
 *
 * @return Returns true on success, else false.
 */
SN_API bool sn_thread_init(void);

/**
 * @brief Shutdown the threading related things.
 *
 * Once this function is called, should not use any other
 * threading functions without initializing again.
 *
 * @return Returns true on success, else false.
 */
SN_API void sn_thread_shutdown(void);

/**
 * @brief Create a thread.
 *
 * @param thread Pointer to thread id.
 * @param func The function that thread runs.
 * @param data The data to the function.
 *
 * @return Returns true on success, else false.
 */
SN_API bool sn_thread_create(snThread *thread, snThreadFn func, void *data);

/**
 * @brief Detach the given thread.
 *
 * @param thread The thread id.
 *
 * @return Returns true on success, else false.
 */
SN_API bool sn_thread_detach(snThread *thread);

/**
 * @brief Join the given thread.
 *
 * Blocks until the thread terminates.
 *
 * @param thread The thread id.
 * @param ret Pointer to store the value returned by thread (can be NULL).
 *
 * @return Returns true on success, else false.
 */
SN_API bool sn_thread_join(snThread *thread, void **ret);

/**
 * @brief Exit from the calling thread.
 *
 * @note This function does not return.
 *
 * @param ret The value to return when thread is joined.
 */
SN_API void sn_thread_exit(void *ret);

/**
 * @brief Obtain the thread id of calling thread.
 *
 * @return Returns the thread id.
 */
SN_API snThread *sn_thread_self(void);

/**
 * @brief Check whether two thread ids are equal.
 *
 * @param t1 Thread id to compare.
 * @param t2 Thread id to compare.
 */
SN_API bool sn_thread_equal(const snThread *t1, const snThread *t2);

/**
 * @brief Attach external thread.
 *
 * @param thread Pointer to snThread.
 *
 * @return Returns true on success, else false.
 */
SN_API bool sn_thread_attach(snThread *thread);


#pragma once

#include "snthreads/defines.h"

/**
 * @enum snMemoryOrder
 * @brief Memory order constants.
 *
 * @note The default memory order used by all the sn_atomic_* functions is
 * SN_MEMORY_ORDER_NONE.
 */
typedef enum snMemoryOrder {
    SN_MEMORY_ORDER_NONE, /**< No memory order */
    SN_MEMORY_ORDER_RELAXED =
        SN_MEMORY_ORDER_NONE, /**< Alias for SN_MEMORY_ORDER_NONE */
    SN_MEMORY_ORDER_ACQUIRE, /**< Acquire operation */
    SN_MEMORY_ORDER_RELEASE, /**< Release operation */
    SN_MEMORY_ORDER_ACQ_REL, /**< Both acquire and release operation */
    SN_MEMORY_ORDER_TOTAL_ORDER, /**< Total ordering */
    SN_MEMORY_ORDER_SEQ_CST =
        SN_MEMORY_ORDER_TOTAL_ORDER /**< Alias for SN_MEMORY_ORDER_TOTAL_ORDER
                                     */
} snMemoryOrder;

/**
 * @brief Add a memory fence(barrier).
 *
 * @param fence The type of the memory fence
 */
SN_API void sn_memory_fence(snMemoryOrder fence);

/**
 * @brief Prefix used by the engine for atomic types.
 */
#define SN_ATOMIC_PREFIX sn_atomic_

/**
 * @brief Get the atomic type using the prefix for give data type.
 *
 * @param type Type for which to get the atomic type
 */
#define SN_GET_ATOMIC_TYPE(type) SN_CONCAT_EXPANDED(SN_ATOMIC_PREFIX, type)

/**
 * @brief Get the atomic function for the given type and name.
 *
 * @param name The name like store, load, etc
 * @param type The type for which to get the funciton
 */
#define SN_GET_ATOMIC_FUNCTION(name, type) \
    SN_CONCAT_EXPANDED3(SN_ATOMIC_PREFIX, SN_CONCAT(name, _), type)

/**
 * @brief Atomic flags type.
 */
typedef struct sn_atomic_flag {
        SN_STATIC_ASSERT(alignof(bool) <= 8, "Align of bool is > 8 bytes!");
        alignas(alignof(bool)) volatile bool flag;
} sn_atomic_flag;

/**
 * @brief TAS operation on the sn_atomic_flag with given memory order.
 *
 * Sets the flag and returns the previous value of flag.
 *
 * @param obj The atomic flag on which to perfrom TAS
 * @param memory_order The memory order to use
 *
 * @return Value of the flag before setting it.
 */
SN_API bool sn_atomic_flag_test_and_set_explicit(
    volatile sn_atomic_flag *obj, snMemoryOrder memory_order);

/**
 * @brief Clear(unset) the atomic flag with given memory order.
 *
 * @param obj The atomic flag to be cleared
 * @param memory_order The memory order to use
 */
SN_API void sn_atomic_flag_clear_explicit(volatile sn_atomic_flag *obj,
                                               snMemoryOrder memory_order);

/**
 * @brief Load the value in the atomic flag with given memory order.
 *
 * Memory order must be one of SN_MEMORY_ORDER_NONE, SN_MEMORY_ORDER_ACQUIRE,
 * SN_MEMORY_ORDER_TOTAL_ORDER.
 *
 * @param obj The atomic flag object
 * @param memory_order The memory order to use
 *
 * @return Returns true if flag is set, else false.
 */
SN_API bool sn_atomic_flag_load_explicit(volatile sn_atomic_flag *obj,
                                            snMemoryOrder memory_order);

/**
 * @brief TAS with default memory order.
 * Refer sn_atomic_flag_test_and_set_explicit
 */
#define sn_atomic_flag_test_and_set(obj) \
    sn_atomic_flag_test_and_set_explicit(obj, SN_MEMORY_ORDER_NONE)

/**
 * @brief Clear the atomic flag with default memory order.
 * Refer sn_atomic_flag_clear_explicit
 */
#define sn_atomic_flag_clear(obj) \
    sn_atomic_flag_clear_explicit(obj, SN_MEMORY_ORDER_NONE)

/**
 * @brief Load the value in the atomic flag with default memory order.
 * Refer sn_atomic_flag_load_explicit
 */
#define sn_atomic_flag_load(obj) \
    sn_atomic_flag_load_explicit(obj, SN_MEMORY_ORDER_NONE)

/**
 * @brief Initialize the atomic flag variable.
 */
#define SN_ATOMIC_FLAG_INIT {false}

/**
 * @brief Define a atomic type for given type.
 *
 * @note The align(type) should be less than or equal to 8 bytes.
 */
#define DEFINE_ATOMIC_TYPE(type)                                     \
    typedef struct SN_GET_ATOMIC_TYPE(type) {                           \
            SN_STATIC_ASSERT(                                        \
                alignof(type) <= 8,                                  \
                "Cannot create a atomic type for type with alignof " \
                "> 8 bytes!");                                       \
            alignas(alignof(type)) volatile type value;              \
    } SN_GET_ATOMIC_TYPE(type)

/**
 * @brief Declare atomic load function for given type.
 */
#define DECLARE_ATOMIC_LOAD(type)                     \
    SN_API type SN_GET_ATOMIC_FUNCTION(load, type)( \
        const volatile SN_GET_ATOMIC_TYPE(type) * obj,   \
        snMemoryOrder memory_order)

/**
 * @brief Declare atomic store function for given type.
 */
#define DECLARE_ATOMIC_STORE(type)                        \
    SN_API void SN_GET_ATOMIC_FUNCTION(store, type)(    \
        volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, \
        snMemoryOrder memory_order)

/**
 * @brief Declare atomic exchange function for given type.
 */
#define DECLARE_ATOMIC_EXCHANGE(type)                     \
    SN_API type SN_GET_ATOMIC_FUNCTION(exchange, type)( \
        volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, \
        snMemoryOrder memory_order)

/**
 * @brief Declare atomic compare exchange function for given type.
 */
#define DECLARE_ATOMIC_COMPARE_EXCHANGE(type)                            \
    SN_API bool SN_GET_ATOMIC_FUNCTION(compare_exchange, type)(          \
        volatile SN_GET_ATOMIC_TYPE(type) * obj, type * expect, type value, \
        snMemoryOrder success, snMemoryOrder fail)

/**
 * @brief Declare atomic fetch_add function for given type.
 */
#define DECLARE_ATOMIC_FETCH_ADD(type)                     \
    SN_API type SN_GET_ATOMIC_FUNCTION(fetch_add, type)( \
        volatile SN_GET_ATOMIC_TYPE(type) * obj, type value,  \
        snMemoryOrder memory_order)

/**
 * @brief Declare atomic fetch_sub function for given type.
 */
#define DECLARE_ATOMIC_FETCH_SUB(type)                     \
    SN_API type SN_GET_ATOMIC_FUNCTION(fetch_sub, type)( \
        volatile SN_GET_ATOMIC_TYPE(type) * obj, type value,  \
        snMemoryOrder memory_order)

/**
 * @brief Declare atomic fetch_or function for given type.
 */
#define DECLARE_ATOMIC_FETCH_OR(type)                     \
    SN_API type SN_GET_ATOMIC_FUNCTION(fetch_or, type)( \
        volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, \
        snMemoryOrder memory_order)

/**
 * @brief Declare atomic fetch_xor function for given type.
 */
#define DECLARE_ATOMIC_FETCH_XOR(type)                     \
    SN_API type SN_GET_ATOMIC_FUNCTION(fetch_xor, type)( \
        volatile SN_GET_ATOMIC_TYPE(type) * obj, type value,  \
        snMemoryOrder memory_order)

/**
 * @brief Declare atomic fetch_and function for given type.
 */
#define DECLARE_ATOMIC_FETCH_AND(type)                     \
    SN_API type SN_GET_ATOMIC_FUNCTION(fetch_and, type)( \
        volatile SN_GET_ATOMIC_TYPE(type) * obj, type value,  \
        snMemoryOrder memory_order)

/**
 * @brief Declare all the atomic functions for given type.
 */
#define DECLARE_ATOMIC_FUNCTIONS(type)     \
    DECLARE_ATOMIC_LOAD(type);             \
    DECLARE_ATOMIC_STORE(type);            \
    DECLARE_ATOMIC_EXCHANGE(type);         \
    DECLARE_ATOMIC_COMPARE_EXCHANGE(type); \
    DECLARE_ATOMIC_FETCH_ADD(type);        \
    DECLARE_ATOMIC_FETCH_SUB(type);        \
    DECLARE_ATOMIC_FETCH_OR(type);         \
    DECLARE_ATOMIC_FETCH_XOR(type);        \
    DECLARE_ATOMIC_FETCH_AND(type)

/**
 * @brief Creates the atomic type with all function declarations.
 */
#define CREATE_ATOMIC_TYPE(type) \
    DEFINE_ATOMIC_TYPE(type);    \
    DECLARE_ATOMIC_FUNCTIONS(type)

CREATE_ATOMIC_TYPE(int8_t);
CREATE_ATOMIC_TYPE(int16_t);
CREATE_ATOMIC_TYPE(int32_t);
CREATE_ATOMIC_TYPE(int64_t);

CREATE_ATOMIC_TYPE(int_fast8_t);
CREATE_ATOMIC_TYPE(int_fast16_t);
CREATE_ATOMIC_TYPE(int_fast32_t);
CREATE_ATOMIC_TYPE(int_fast64_t);

CREATE_ATOMIC_TYPE(int_least8_t);
CREATE_ATOMIC_TYPE(int_least16_t);
CREATE_ATOMIC_TYPE(int_least32_t);
CREATE_ATOMIC_TYPE(int_least64_t);

CREATE_ATOMIC_TYPE(uint8_t);
CREATE_ATOMIC_TYPE(uint16_t);
CREATE_ATOMIC_TYPE(uint32_t);
CREATE_ATOMIC_TYPE(uint64_t);

CREATE_ATOMIC_TYPE(uint_fast8_t);
CREATE_ATOMIC_TYPE(uint_fast16_t);
CREATE_ATOMIC_TYPE(uint_fast32_t);
CREATE_ATOMIC_TYPE(uint_fast64_t);

CREATE_ATOMIC_TYPE(uint_least8_t);
CREATE_ATOMIC_TYPE(uint_least16_t);
CREATE_ATOMIC_TYPE(uint_least32_t);
CREATE_ATOMIC_TYPE(uint_least64_t);

/**
 * @brief Get the atomic function for given object type.
 */
#define SN_GET_GENERIC_ATOMIC_FUNCTION(obj, name)                    \
    _Generic((obj),                                               \
        SN_GET_ATOMIC_TYPE(int8_t) *: SN_GET_ATOMIC_FUNCTION(name, int8_t),     \
        SN_GET_ATOMIC_TYPE(int16_t) *: SN_GET_ATOMIC_FUNCTION(name, int16_t),   \
        SN_GET_ATOMIC_TYPE(int32_t) *: SN_GET_ATOMIC_FUNCTION(name, int32_t),   \
        SN_GET_ATOMIC_TYPE(int64_t) *: SN_GET_ATOMIC_FUNCTION(name, int64_t),   \
                                                                  \
        SN_GET_ATOMIC_TYPE(int_fast8_t) *: SN_GET_ATOMIC_FUNCTION(name, int_fast8_t),   \
        SN_GET_ATOMIC_TYPE(int_fast16_t) *: SN_GET_ATOMIC_FUNCTION(name, int_fast16_t), \
        SN_GET_ATOMIC_TYPE(int_fast32_t) *: SN_GET_ATOMIC_FUNCTION(name, int_fast32_t), \
        SN_GET_ATOMIC_TYPE(int_fast64_t) *: SN_GET_ATOMIC_FUNCTION(name, int_fast64_t), \
                                                                  \
        SN_GET_ATOMIC_TYPE(int_least8_t) *: SN_GET_ATOMIC_FUNCTION(name, int_least8_t),   \
        SN_GET_ATOMIC_TYPE(int_least16_t) *: SN_GET_ATOMIC_FUNCTION(name, int_least16_t), \
        SN_GET_ATOMIC_TYPE(int_least32_t) *: SN_GET_ATOMIC_FUNCTION(name, int_least32_t), \
        SN_GET_ATOMIC_TYPE(int_least64_t) *: SN_GET_ATOMIC_FUNCTION(name, int_least64_t), \
                                                                  \
        SN_GET_ATOMIC_TYPE(uint8_t) *: SN_GET_ATOMIC_FUNCTION(name, uint8_t),     \
        SN_GET_ATOMIC_TYPE(uint16_t) *: SN_GET_ATOMIC_FUNCTION(name, uint16_t),   \
        SN_GET_ATOMIC_TYPE(uint32_t) *: SN_GET_ATOMIC_FUNCTION(name, uint32_t),   \
        SN_GET_ATOMIC_TYPE(uint64_t) *: SN_GET_ATOMIC_FUNCTION(name, uint64_t),   \
                                                                  \
        SN_GET_ATOMIC_TYPE(uint_fast8_t) *: SN_GET_ATOMIC_FUNCTION(name, uint_fast8_t),   \
        SN_GET_ATOMIC_TYPE(uint_fast16_t) *: SN_GET_ATOMIC_FUNCTION(name, uint_fast16_t), \
        SN_GET_ATOMIC_TYPE(uint_fast32_t) *: SN_GET_ATOMIC_FUNCTION(name, uint_fast32_t), \
        SN_GET_ATOMIC_TYPE(uint_fast64_t) *: SN_GET_ATOMIC_FUNCTION(name, uint_fast64_t), \
                                                                  \
        SN_GET_ATOMIC_TYPE(uint_least8_t) *: SN_GET_ATOMIC_FUNCTION(name, uint_least8_t),   \
        SN_GET_ATOMIC_TYPE(uint_least16_t) *: SN_GET_ATOMIC_FUNCTION(name, uint_least16_t), \
        SN_GET_ATOMIC_TYPE(uint_least32_t) *: SN_GET_ATOMIC_FUNCTION(name, uint_least32_t), \
        SN_GET_ATOMIC_TYPE(uint_least64_t) *: SN_GET_ATOMIC_FUNCTION(name, uint_least64_t), \
        default: NULL)

/**
 * @brief Initialize the newly created atomic variable.
 *
 * @param value The value for initializing
 */
#define SN_ATOMIC_VAR_INIT(value) {value}

/**
 * @brief Atomic load operation.
 *
 * Memory order should be one of SN_MEMORY_ORDER_NONE, SN_MEMORY_ORDER_ACQUIRE,
 * SN_MEMORY_ORDER_TOTAL_ORDER.
 */
#define sn_atomic_load_explicit(obj, memory_order) \
    SN_GET_GENERIC_ATOMIC_FUNCTION(obj, load)(obj, memory_order)

/**
 * @brief Atomic store operation.
 *
 * Memory order should be one of SN_MEMORY_ORDER_NONE, SN_MEMORY_ORDER_RELEASE,
 * SN_MEMORY_ORDER_TOTAL_ORDER.
 */
#define sn_atomic_store_explicit(obj, value, memory_order) \
    SN_GET_GENERIC_ATOMIC_FUNCTION(obj, store)(obj, value, memory_order)

/**
 * @brief Atomic exchange operation.
 */
#define sn_atomic_exchange_explicit(obj, value, memory_order) \
    SN_GET_GENERIC_ATOMIC_FUNCTION(obj, exchange)(obj, value, memory_order)

/**
 * @brief Atomic comapre exchange operation.
 */
#define sn_atomic_compare_exchange_explicit(obj, expect, value, success, fail) \
    get_generic_atomic_function(obj, compare_exchange)(obj, expect, value,     \
                                                       success, fail)

/**
 * @brief Atomic fetch_add operation.
 */
#define sn_atomic_fetch_add_explicit(obj, value, memory_order) \
    SN_GET_GENERIC_ATOMIC_FUNCTION(obj, fetch_add)(obj, value, memory_order)

/**
 * @brief Atomic fetch_sub operation.
 */
#define sn_atomic_fetch_sub_explicit(obj, value, memory_order) \
    SN_GET_GENERIC_ATOMIC_FUNCTION(obj, fetch_sub)(obj, value, memory_order)

/**
 * @brief Atomic fetch_or operation.
 */
#define sn_atomic_fetch_or_explicit(obj, value, memory_order) \
    SN_GET_GENERIC_ATOMIC_FUNCTION(obj, fetch_or)(obj, value, memory_order)

/**
 * @brief Atomic fetch_xor operation.
 */
#define sn_atomic_fetch_xor_explicit(obj, value, memory_order) \
    SN_GET_GENERIC_ATOMIC_FUNCTION(obj, fetch_xor)(obj, value, memory_order)

/**
 * @brief Atomic fetch_and operation.
 */
#define sn_atomic_fetch_and_explicit(obj, value, memory_order) \
    SN_GET_GENERIC_ATOMIC_FUNCTION(obj, fetch_and)(obj, value, memory_order)

/**
 * @brief Atomic load operation with default memory order.
 *
 * @note Default memory order is SN_MEMORY_ORDER_NONE.
 *
 * @param obj Atomic object to load the value from
 *
 * @return Returns value stored in the atomic object.
 */
#define sn_atomic_load(obj) \
    SN_GET_GENERIC_ATOMIC_FUNCTION(obj, load)(obj, SN_MEMORY_ORDER_NONE)

/**
 * @brief Atomic stroe operation with default memory order.
 *
 * @note Default memory order is SN_MEMORY_ORDER_NONE.
 *
 * @param obj Atomic object to store the value
 * @param value The value to store
 */
#define sn_atomic_store(obj, value) \
    SN_GET_GENERIC_ATOMIC_FUNCTION(obj, store)(obj, value, SN_MEMORY_ORDER_NONE)

/**
 * @brief Atomic exchange operation with default memory order.
 *
 * @note Default memory order is SN_MEMORY_ORDER_NONE.
 *
 * @param obj The atomic object
 * @param value The value to exchange with
 *
 * @return Returns the previous value in the atomic object.
 */
#define sn_atomic_exchange(obj, value) \
    SN_GET_GENERIC_ATOMIC_FUNCTION(obj, exchange)(obj, value, SN_MEMORY_ORDER_NONE)

/**
 * @brief Atomic compare exchange operation with default memory order.
 *
 * Exchanges the object's value with given value if the current value of object
 * is the expected value. If expected value is not what is stored in the object
 * then copies the current value of object to expect variable.
 *
 * @note Default memory order is SN_MEMORY_ORDER_NONE.
 *
 * @param obj The atomic object
 * @param expect The value expected in the object
 * @param value Value to exchange with
 *
 * @return Returns true if the value is exchanged else false.
 */
#define sn_atomic_compare_exchange(obj, expect, value)  \
    SN_GET_GENERIC_ATOMIC_FUNCTION(obj, compare_exchange)( \
        obj, expect, value, SN_MEMORY_ORDER_NONE, SN_MEMORY_ORDER_NONE)

/**
 * @brief Atomic fetch_add operation with default memory order.
 *
 * @note Default memory order is SN_MEMORY_ORDER_NONE.
 *
 * @param obj The atomic object
 * @param value The value to add
 *
 * @return Previous value in the atomic object.
 */
#define sn_atomic_fetch_add(obj, value)                     \
    SN_GET_GENERIC_ATOMIC_FUNCTION(obj, fetch_add)(obj, value, \
                                                SN_MEMORY_ORDER_NONE)

/**
 * @brief Atomic fetch_sub operation with default memory order.
 *
 * @note Default memory order is SN_MEMORY_ORDER_NONE.
 *
 * @param obj The atomic object
 * @param value The value to subtract
 *
 * @return Previous value of the atomic object.
 */
#define sn_atomic_fetch_sub(obj, value)                     \
    SN_GET_GENERIC_ATOMIC_FUNCTION(obj, fetch_sub)(obj, value, \
                                                SN_MEMORY_ORDER_NONE)

/**
 * @brief Atomic fetch_or operation with default memory order.
 *
 * @note Default memory order is SN_MEMORY_ORDER_NONE.
 *
 * @param obj The atomic object
 * @param value The value to or with
 *
 * @return Returns the previous value of atomic object.
 */
#define sn_atomic_fetch_or(obj, value) \
    SN_GET_GENERIC_ATOMIC_FUNCTION(obj, fetch_or)(obj, value, SN_MEMORY_ORDER_NONE)

/**
 * @brief Atomic fetch_xor operation with default memory order.
 *
 * @note Default memory order is SN_MEMORY_ORDER_NONE.
 *
 * @param obj The atomic object
 * @param value The value to xor with
 *
 * @return Returns the previous value of atomic object.
 */
#define sn_atomic_fetch_xor(obj, value)                     \
    SN_GET_GENERIC_ATOMIC_FUNCTION(obj, fetch_xor)(obj, value, \
                                                SN_MEMORY_ORDER_NONE)

/**
 * @brief Atomic fetch_and operation with default memory order.
 *
 * @note Default memory order is SN_MEMORY_ORDER_NONE.
 *
 * @param obj The atomic object
 * @param value The value to and with
 *
 * @return Returns the previous value of atomic object.
 */
#define sn_atomic_fetch_and(obj, value)                     \
    SN_GET_GENERIC_ATOMIC_FUNCTION(obj, fetch_and)(obj, value, \
                                                SN_MEMORY_ORDER_NONE)

#undef DEFINE_ATOMIC_TYPE
#undef DECLARE_ATOMIC_LOAD
#undef DECLARE_ATOMIC_STORE
#undef DECLARE_ATOMIC_EXCHANGE
#undef DECLARE_ATOMIC_COMPARE_EXCHANGE
#undef DECLARE_ATOMIC_FETCH_ADD
#undef DECLARE_ATOMIC_FETCH_SUB
#undef DECLARE_ATOMIC_FETCH_OR
#undef DECLARE_ATOMIC_FETCH_XOR
#undef DECLARE_ATOMIC_FETCH_AND
#undef DECLARE_ATOMIC_FUNCTIONS
#undef CREATE_ATOMIC_TYPE

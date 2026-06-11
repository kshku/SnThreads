#include "snthreads/atomics.h"

#ifdef SN_ARCH_AARCH64

    #if defined(SN_COMPILER_MSVC)

        #include "../atomics_shared.h"

    #else  // GCC/Clang

        #define DMB_ISH __asm__ volatile("dmb ish" ::: "memory")
        #define DMB_ISHLD __asm__ volatile("dmb ishld" ::: "memory")
        #define DMB_ISHST __asm__ volatile("dmb ishst" ::: "memory")

        #define PRE_ATOMIC_LOAD_FENCE(memory_order)                       \
            do                                                            \
                if (memory_order == SN_MEMORY_ORDER_TOTAL_ORDER) DMB_ISH; \
            while (0)

        #define POST_ATOMIC_LOAD_FENCE(memory_order)                                                        \
            do                                                                                              \
                if (memory_order == SN_MEMORY_ORDER_ACQUIRE || memory_order == SN_MEMORY_ORDER_TOTAL_ORDER) \
                    DMB_ISHLD;                                                                              \
            while (0)

        #define PRE_ATOMIC_STORE_FENCE(memory_order)                                                        \
            do                                                                                              \
                if (memory_order == SN_MEMORY_ORDER_RELEASE || memory_order == SN_MEMORY_ORDER_TOTAL_ORDER) \
                    DMB_ISHST;                                                                              \
            while (0)

        #define POST_ATOMIC_STORE_FENCE(memory_order)                     \
            do                                                            \
                if (memory_order == SN_MEMORY_ORDER_TOTAL_ORDER) DMB_ISH; \
            while (0)

        #define PRE_ATOMIC_RMW_FENCE(memory_order)                                                      \
            do                                                                                          \
                if (memory_order == SN_MEMORY_ORDER_RELEASE || memory_order == SN_MEMORY_ORDER_ACQ_REL) \
                    DMB_ISHST;                                                                          \
                else if (memory_order == SN_MEMORY_ORDER_TOTAL_ORDER) DMB_ISH;                          \
            while (0)

        #define POST_ATOMIC_RMW_FENCE(memory_order)                                                     \
            do                                                                                          \
                if (memory_order == SN_MEMORY_ORDER_ACQUIRE || memory_order == SN_MEMORY_ORDER_ACQ_REL) \
                    DMB_ISHLD;                                                                          \
                else if (memory_order == SN_MEMORY_ORDER_TOTAL_ORDER) DMB_ISH;                          \
            while (0)

        #define DEFINE_ATOMIC_LOAD(type)                                                     \
            type SN_GET_ATOMIC_FUNCTION(load, type)(                                         \
                const volatile SN_GET_ATOMIC_TYPE(type) * obj, SnMemoryOrder memory_order) { \
                PRE_ATOMIC_LOAD_FENCE(memory_order);                                         \
                type value;                                                                  \
                if (sizeof(type) == 1)                                                       \
                    __asm__ volatile("ldrb %w[value], %[obj]"                                \
                                     : [value] "=r"(value)                                   \
                                     : [obj] "Q"(obj->value));                               \
                else if (sizeof(type) == 2)                                                  \
                    __asm__ volatile("ldrh %w[value], %[obj]"                                \
                                     : [value] "=r"(value)                                   \
                                     : [obj] "Q"(obj->value));                               \
                else                                                                         \
                    __asm__ volatile("ldr %[value], %[obj]"                                  \
                                     : [value] "=r"(value)                                   \
                                     : [obj] "Q"(obj->value));                               \
                POST_ATOMIC_LOAD_FENCE(memory_order);                                        \
                return value;                                                                \
            }

        #define DEFINE_ATOMIC_STORE(type)                                                          \
            void SN_GET_ATOMIC_FUNCTION(store, type)(                                              \
                volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
                PRE_ATOMIC_STORE_FENCE(memory_order);                                              \
                if (sizeof(type) == 1)                                                             \
                    __asm__ volatile("strb %w[value], %[obj]"                                      \
                                     : [obj] "=Q"(obj->value)                                      \
                                     : [value] "r"(value)                                          \
                                     : "memory");                                                  \
                else if (sizeof(type) == 2)                                                        \
                    __asm__ volatile("strh %w[value], %[obj]"                                      \
                                     : [obj] "=Q"(obj->value)                                      \
                                     : [value] "r"(value)                                          \
                                     : "memory");                                                  \
                else                                                                               \
                    __asm__ volatile("str %[value], %[obj]"                                        \
                                     : [obj] "=Q"(obj->value)                                      \
                                     : [value] "r"(value)                                          \
                                     : "memory");                                                  \
                POST_ATOMIC_STORE_FENCE(memory_order);                                             \
            }

        #define DEFINE_ATOMIC_EXCHANGE(type)                                                       \
            type SN_GET_ATOMIC_FUNCTION(exchange, type)(                                           \
                volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
                SN_UNUSED(memory_order);                                                           \
                type old;                                                                          \
                unsigned int status;                                                               \
                __asm__ volatile(                                                                  \
                    "dmb ish\n\t"                                                                  \
                    "1: ldaxr %[old], %[obj]\n\t"                                                  \
                    "stlxr %w[status], %[value], %[obj]\n\t"                                       \
                    "cbnz %w[status], 1b\n\t"                                                      \
                    "dmb ish\n\t"                                                                  \
                    : [old] "=&r"(old), [status] "=&r"(status), [obj] "+Q"(obj->value)             \
                    : [value] "r"(value)                                                           \
                    : "memory");                                                                   \
                return old;                                                                        \
            }

        #define DEFINE_ATOMIC_COMPARE_EXCHANGE(type)                                   \
            bool SN_GET_ATOMIC_FUNCTION(compare_exchange, type)(                       \
                volatile SN_GET_ATOMIC_TYPE(type) * obj, type * expect, type value,    \
                SnMemoryOrder success, SnMemoryOrder fail) {                           \
                SN_UNUSED(success);                                                    \
                SN_UNUSED(fail);                                                       \
                type old;                                                              \
                unsigned int status;                                                   \
                type expected = *expect;                                               \
                __asm__ volatile(                                                      \
                    "dmb ish\n\t"                                                      \
                    "1: ldaxr %[old], %[obj]\n\t"                                      \
                    "cmp %[old], %[expected]\n\t"                                      \
                    "bne 2f\n\t"                                                       \
                    "stlxr %w[status], %[value], %[obj]\n\t"                           \
                    "cbnz %w[status], 1b\n\t"                                          \
                    "dmb ish\n\t"                                                      \
                    "2:\n\t"                                                           \
                    : [old] "=&r"(old), [status] "=&r"(status), [obj] "+Q"(obj->value) \
                    : [expected] "r"(expected), [value] "r"(value)                     \
                    : "cc", "memory");                                                 \
                if (old == expected) return true;                                      \
                *expect = old;                                                         \
                return false;                                                          \
            }

        #define DEFINE_ATOMIC_FETCH_ADD(type)                                                                \
            type SN_GET_ATOMIC_FUNCTION(fetch_add, type)(                                                    \
                volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) {           \
                SN_UNUSED(memory_order);                                                                     \
                type old;                                                                                    \
                type new_val;                                                                                \
                unsigned int status;                                                                         \
                __asm__ volatile(                                                                            \
                    "dmb ish\n\t"                                                                            \
                    "1: ldaxr %[old], %[obj]\n\t"                                                            \
                    "add %[new], %[old], %[val]\n\t"                                                         \
                    "stlxr %w[status], %[new], %[obj]\n\t"                                                   \
                    "cbnz %w[status], 1b\n\t"                                                                \
                    "dmb ish\n\t"                                                                            \
                    : [old] "=&r"(old), [new] "=&r"(new_val), [status] "=&r"(status), [obj] "+Q"(obj->value) \
                    : [val] "r"(value)                                                                       \
                    : "memory");                                                                             \
                return old;                                                                                  \
            }

        #define DEFINE_ATOMIC_FETCH_SUB(type)                                                                \
            type SN_GET_ATOMIC_FUNCTION(fetch_sub, type)(                                                    \
                volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) {           \
                SN_UNUSED(memory_order);                                                                     \
                type old;                                                                                    \
                type new_val;                                                                                \
                unsigned int status;                                                                         \
                __asm__ volatile(                                                                            \
                    "dmb ish\n\t"                                                                            \
                    "1: ldaxr %[old], %[obj]\n\t"                                                            \
                    "sub %[new], %[old], %[val]\n\t"                                                         \
                    "stlxr %w[status], %[new], %[obj]\n\t"                                                   \
                    "cbnz %w[status], 1b\n\t"                                                                \
                    "dmb ish\n\t"                                                                            \
                    : [old] "=&r"(old), [new] "=&r"(new_val), [status] "=&r"(status), [obj] "+Q"(obj->value) \
                    : [val] "r"(value)                                                                       \
                    : "memory");                                                                             \
                return old;                                                                                  \
            }

void sn_memory_fence(SnMemoryOrder fence) {
    switch (fence) {
        case SN_MEMORY_ORDER_NONE:
            break;
        case SN_MEMORY_ORDER_ACQUIRE:
            DMB_ISHLD;
            break;
        case SN_MEMORY_ORDER_RELEASE:
            DMB_ISHST;
            break;
        case SN_MEMORY_ORDER_ACQ_REL:
        case SN_MEMORY_ORDER_TOTAL_ORDER:
            DMB_ISH;
            break;
    }
}

bool sn_atomic_flag_test_and_set_explicit(volatile sn_atomic_flag *obj, SnMemoryOrder memory_order) {
    SN_UNUSED(memory_order);
    bool ret = false;
    unsigned int status;
    __asm__ volatile(
        "dmb ish\n\t"
        "1: ldaxrb %w[ret], %[flag]\n\t"
        "stlxrb %w[status], %w[one], %[flag]\n\t"
        "cbnz %w[status], 1b\n\t"
        "dmb ish\n\t"
        : [ret] "=&r"(ret), [status] "=&r"(status), [flag] "+Q"(obj->flag)
        : [one] "r"((bool)true)
        : "memory");
    return ret;
}

void sn_atomic_flag_clear_explicit(volatile sn_atomic_flag *obj, SnMemoryOrder memory_order) {
    PRE_ATOMIC_RMW_FENCE(memory_order);
    bool reset = false;
    __asm__ volatile("strb %w[value], %[flag]"
                     : [flag] "=Q"(obj->flag)
                     : [value] "r"(reset)
                     : "memory");
    POST_ATOMIC_RMW_FENCE(memory_order);
}

bool sn_atomic_flag_load_explicit(volatile sn_atomic_flag *obj, SnMemoryOrder memory_order) {
    PRE_ATOMIC_LOAD_FENCE(memory_order);
    bool ret;
    __asm__ volatile("ldrb %w[value], %[flag]" : [value] "=r"(ret) : [flag] "Q"(obj->flag));
    POST_ATOMIC_LOAD_FENCE(memory_order);
    return ret;
}

        #include "../atomics_shared.h"

    #endif  // !SN_COMPILER_MSVC

#endif  // SN_ARCH_AARCH64

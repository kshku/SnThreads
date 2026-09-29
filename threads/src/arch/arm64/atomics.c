#include "snthreads/atomics.h"

#ifdef SN_ARCH_ARM64

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

    /* AArch64 has no 8 or 16 bit general purpose registers, and every
       access form comes in a byte, halfword, word and doubleword variant.
       Each width therefore gets its own instruction and its own fixed
       width temporary, so the constraint and the register always agree
       and a narrow object is never touched as if it were 8 bytes wide. The
       type itself only appears in the final narrowing. */

        #define SN_ARM64_LOAD_1(type)                                                              \
            unsigned int wide;                                                                     \
            __asm__ volatile("ldrb %w[wide], %[obj]" : [wide] "=r"(wide) : [obj] "Q"(obj->value)); \
            return (type)wide;

        #define SN_ARM64_LOAD_2(type)                                                              \
            unsigned int wide;                                                                     \
            __asm__ volatile("ldrh %w[wide], %[obj]" : [wide] "=r"(wide) : [obj] "Q"(obj->value)); \
            return (type)wide;

        #define SN_ARM64_LOAD_4(type)                                                             \
            unsigned int wide;                                                                    \
            __asm__ volatile("ldr %w[wide], %[obj]" : [wide] "=r"(wide) : [obj] "Q"(obj->value)); \
            return (type)wide;

        #define SN_ARM64_LOAD_8(type)                                                            \
            uint64_t wide;                                                                       \
            __asm__ volatile("ldr %[wide], %[obj]" : [wide] "=r"(wide) : [obj] "Q"(obj->value)); \
            return (type)wide;

        #define SN_ARM64_STORE_1(type, value)          \
            unsigned int wide = (unsigned int)(value); \
            __asm__ volatile("strb %w[wide], %[obj]"   \
                             : [obj] "=Q"(obj->value)  \
                             : [wide] "r"(wide)        \
                             : "memory");

        #define SN_ARM64_STORE_2(type, value)          \
            unsigned int wide = (unsigned int)(value); \
            __asm__ volatile("strh %w[wide], %[obj]"   \
                             : [obj] "=Q"(obj->value)  \
                             : [wide] "r"(wide)        \
                             : "memory");

        #define SN_ARM64_STORE_4(type, value)          \
            unsigned int wide = (unsigned int)(value); \
            __asm__ volatile("str %w[wide], %[obj]"    \
                             : [obj] "=Q"(obj->value)  \
                             : [wide] "r"(wide)        \
                             : "memory");

        #define SN_ARM64_STORE_8(type, value)         \
            uint64_t wide = (uint64_t)(value);        \
            __asm__ volatile("str %[wide], %[obj]"    \
                             : [obj] "=Q"(obj->value) \
                             : [wide] "r"(wide)       \
                             : "memory");

        #define DEFINE_ATOMIC_LOAD(type)                                                     \
            type SN_GET_ATOMIC_FUNCTION(load, type)(                                         \
                const volatile SN_GET_ATOMIC_TYPE(type) * obj, SnMemoryOrder memory_order) { \
                PRE_ATOMIC_LOAD_FENCE(memory_order);                                         \
                if (sizeof(type) == 1) {                                                     \
                    SN_ARM64_LOAD_1(type);                                                   \
                }                                                                            \
                if (sizeof(type) == 2) {                                                     \
                    SN_ARM64_LOAD_2(type);                                                   \
                }                                                                            \
                if (sizeof(type) == 4) {                                                     \
                    SN_ARM64_LOAD_4(type);                                                   \
                }                                                                            \
                if (sizeof(type) == 8) {                                                     \
                    SN_ARM64_LOAD_8(type);                                                   \
                }                                                                            \
            }

        #define DEFINE_ATOMIC_STORE(type)                                                          \
            void SN_GET_ATOMIC_FUNCTION(store, type)(                                              \
                volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
                PRE_ATOMIC_STORE_FENCE(memory_order);                                              \
                if (sizeof(type) == 1) {                                                           \
                    SN_ARM64_STORE_1(type, value);                                                 \
                }                                                                                  \
                if (sizeof(type) == 2) {                                                           \
                    SN_ARM64_STORE_2(type, value);                                                 \
                }                                                                                  \
                if (sizeof(type) == 4) {                                                           \
                    SN_ARM64_STORE_4(type, value);                                                 \
                }                                                                                  \
                if (sizeof(type) == 8) {                                                           \
                    SN_ARM64_STORE_8(type, value);                                                 \
                }                                                                                  \
                POST_ATOMIC_STORE_FENCE(memory_order);                                             \
            }

        #define SN_ARM64_EXCHANGE_1(type, obj, value)                                        \
            unsigned int wide_value = (unsigned int)(value);                                 \
            unsigned int wide_old;                                                           \
            unsigned int status;                                                             \
            __asm__ volatile(                                                                \
                "dmb ish\n\t"                                                                \
                "1: ldaxrb %w[wide_old], %[obj]\n\t"                                         \
                "stlxrb %w[status], %w[wide_value], %[obj]\n\t"                              \
                "cbnz %w[status], 1b\n\t"                                                    \
                "dmb ish\n\t"                                                                \
                : [wide_old] "=&r"(wide_old), [status] "=&r"(status), [obj] "+Q"(obj->value) \
                : [wide_value] "r"(wide_value)                                               \
                : "memory");                                                                 \
            return (type)wide_old;

        #define SN_ARM64_EXCHANGE_2(type, obj, value)                                        \
            unsigned int wide_value = (unsigned int)(value);                                 \
            unsigned int wide_old;                                                           \
            unsigned int status;                                                             \
            __asm__ volatile(                                                                \
                "dmb ish\n\t"                                                                \
                "1: ldaxrh %w[wide_old], %[obj]\n\t"                                         \
                "stlxrh %w[status], %w[wide_value], %[obj]\n\t"                              \
                "cbnz %w[status], 1b\n\t"                                                    \
                "dmb ish\n\t"                                                                \
                : [wide_old] "=&r"(wide_old), [status] "=&r"(status), [obj] "+Q"(obj->value) \
                : [wide_value] "r"(wide_value)                                               \
                : "memory");                                                                 \
            return (type)wide_old;

        #define SN_ARM64_EXCHANGE_4(type, obj, value)                                        \
            unsigned int wide_value = (unsigned int)(value);                                 \
            unsigned int wide_old;                                                           \
            unsigned int status;                                                             \
            __asm__ volatile(                                                                \
                "dmb ish\n\t"                                                                \
                "1: ldaxr %w[wide_old], %[obj]\n\t"                                          \
                "stlxr %w[status], %w[wide_value], %[obj]\n\t"                               \
                "cbnz %w[status], 1b\n\t"                                                    \
                "dmb ish\n\t"                                                                \
                : [wide_old] "=&r"(wide_old), [status] "=&r"(status), [obj] "+Q"(obj->value) \
                : [wide_value] "r"(wide_value)                                               \
                : "memory");                                                                 \
            return (type)wide_old;

        #define SN_ARM64_EXCHANGE_8(type, obj, value)                                        \
            uint64_t wide_value = (uint64_t)(value);                                         \
            uint64_t wide_old;                                                               \
            unsigned int status;                                                             \
            __asm__ volatile(                                                                \
                "dmb ish\n\t"                                                                \
                "1: ldaxr %[wide_old], %[obj]\n\t"                                           \
                "stlxr %w[status], %[wide_value], %[obj]\n\t"                                \
                "cbnz %w[status], 1b\n\t"                                                    \
                "dmb ish\n\t"                                                                \
                : [wide_old] "=&r"(wide_old), [status] "=&r"(status), [obj] "+Q"(obj->value) \
                : [wide_value] "r"(wide_value)                                               \
                : "memory");                                                                 \
            return (type)wide_old;

        #define DEFINE_ATOMIC_EXCHANGE(type)                                                       \
            type SN_GET_ATOMIC_FUNCTION(exchange, type)(                                           \
                volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
                SN_UNUSED(memory_order);                                                           \
                if (sizeof(type) == 1) {                                                           \
                    SN_ARM64_EXCHANGE_1(type, obj, value);                                         \
                }                                                                                  \
                if (sizeof(type) == 2) {                                                           \
                    SN_ARM64_EXCHANGE_2(type, obj, value);                                         \
                }                                                                                  \
                if (sizeof(type) == 4) {                                                           \
                    SN_ARM64_EXCHANGE_4(type, obj, value);                                         \
                }                                                                                  \
                if (sizeof(type) == 8) {                                                           \
                    SN_ARM64_EXCHANGE_8(type, obj, value);                                         \
                }                                                                                  \
            }

        #define SN_ARM64_CAS_1(type, obj, expect, value)                                     \
            unsigned int wide_expected = (unsigned int)(*(expect));                          \
            unsigned int wide_old;                                                           \
            unsigned int wide_value = (unsigned int)(value);                                 \
            unsigned int status;                                                             \
            bool swapped;                                                                    \
            __asm__ volatile(                                                                \
                "dmb ish\n\t"                                                                \
                "1: ldaxrb %w[wide_old], %[obj]\n\t"                                         \
                "cmp %w[wide_old], %w[wide_expected]\n\t"                                    \
                "b.ne 2f\n\t"                                                                \
                "stlxrb %w[status], %w[wide_value], %[obj]\n\t"                              \
                "cbnz %w[status], 1b\n\t"                                                    \
                "dmb ish\n\t"                                                                \
                "2:\n\t"                                                                     \
                : [wide_old] "=&r"(wide_old), [status] "=&r"(status), [obj] "+Q"(obj->value) \
                : [wide_expected] "r"(wide_expected), [wide_value] "r"(wide_value)           \
                : "cc", "memory");                                                           \
            swapped = ((type)wide_old == (type)wide_expected);                               \
            if (!swapped) *(expect) = (type)wide_old;                                        \
            return swapped;

        #define SN_ARM64_CAS_2(type, obj, expect, value)                                     \
            unsigned int wide_expected = (unsigned int)(*(expect));                          \
            unsigned int wide_old;                                                           \
            unsigned int wide_value = (unsigned int)(value);                                 \
            unsigned int status;                                                             \
            bool swapped;                                                                    \
            __asm__ volatile(                                                                \
                "dmb ish\n\t"                                                                \
                "1: ldaxrh %w[wide_old], %[obj]\n\t"                                         \
                "cmp %w[wide_old], %w[wide_expected]\n\t"                                    \
                "b.ne 2f\n\t"                                                                \
                "stlxrh %w[status], %w[wide_value], %[obj]\n\t"                              \
                "cbnz %w[status], 1b\n\t"                                                    \
                "dmb ish\n\t"                                                                \
                "2:\n\t"                                                                     \
                : [wide_old] "=&r"(wide_old), [status] "=&r"(status), [obj] "+Q"(obj->value) \
                : [wide_expected] "r"(wide_expected), [wide_value] "r"(wide_value)           \
                : "cc", "memory");                                                           \
            swapped = ((type)wide_old == (type)wide_expected);                               \
            if (!swapped) *(expect) = (type)wide_old;                                        \
            return swapped;

        #define SN_ARM64_CAS_4(type, obj, expect, value)                                     \
            unsigned int wide_expected = (unsigned int)(*(expect));                          \
            unsigned int wide_old;                                                           \
            unsigned int wide_value = (unsigned int)(value);                                 \
            unsigned int status;                                                             \
            bool swapped;                                                                    \
            __asm__ volatile(                                                                \
                "dmb ish\n\t"                                                                \
                "1: ldaxr %w[wide_old], %[obj]\n\t"                                          \
                "cmp %w[wide_old], %w[wide_expected]\n\t"                                    \
                "b.ne 2f\n\t"                                                                \
                "stlxr %w[status], %w[wide_value], %[obj]\n\t"                               \
                "cbnz %w[status], 1b\n\t"                                                    \
                "dmb ish\n\t"                                                                \
                "2:\n\t"                                                                     \
                : [wide_old] "=&r"(wide_old), [status] "=&r"(status), [obj] "+Q"(obj->value) \
                : [wide_expected] "r"(wide_expected), [wide_value] "r"(wide_value)           \
                : "cc", "memory");                                                           \
            swapped = ((type)wide_old == (type)wide_expected);                               \
            if (!swapped) *(expect) = (type)wide_old;                                        \
            return swapped;

        #define SN_ARM64_CAS_8(type, obj, expect, value)                                     \
            uint64_t wide_expected = (uint64_t)(*(expect));                                  \
            uint64_t wide_old;                                                               \
            uint64_t wide_value = (uint64_t)(value);                                         \
            unsigned int status;                                                             \
            bool swapped;                                                                    \
            __asm__ volatile(                                                                \
                "dmb ish\n\t"                                                                \
                "1: ldaxr %[wide_old], %[obj]\n\t"                                           \
                "cmp %[wide_old], %[wide_expected]\n\t"                                      \
                "b.ne 2f\n\t"                                                                \
                "stlxr %w[status], %[wide_value], %[obj]\n\t"                                \
                "cbnz %w[status], 1b\n\t"                                                    \
                "dmb ish\n\t"                                                                \
                "2:\n\t"                                                                     \
                : [wide_old] "=&r"(wide_old), [status] "=&r"(status), [obj] "+Q"(obj->value) \
                : [wide_expected] "r"(wide_expected), [wide_value] "r"(wide_value)           \
                : "cc", "memory");                                                           \
            swapped = ((type)wide_old == (type)wide_expected);                               \
            if (!swapped) *(expect) = (type)wide_old;                                        \
            return swapped;

        #define DEFINE_ATOMIC_COMPARE_EXCHANGE(type)                                \
            bool SN_GET_ATOMIC_FUNCTION(compare_exchange, type)(                    \
                volatile SN_GET_ATOMIC_TYPE(type) * obj, type * expect, type value, \
                SnMemoryOrder success, SnMemoryOrder fail) {                        \
                SN_UNUSED(success);                                                 \
                SN_UNUSED(fail);                                                    \
                if (sizeof(type) == 1) {                                            \
                    SN_ARM64_CAS_1(type, obj, expect, value);                       \
                }                                                                   \
                if (sizeof(type) == 2) {                                            \
                    SN_ARM64_CAS_2(type, obj, expect, value);                       \
                }                                                                   \
                if (sizeof(type) == 4) {                                            \
                    SN_ARM64_CAS_4(type, obj, expect, value);                       \
                }                                                                   \
                if (sizeof(type) == 8) {                                            \
                    SN_ARM64_CAS_8(type, obj, expect, value);                       \
                }                                                                   \
            }

        #define SN_ARM64_FETCH_1(type, obj, value, instruction)                                                          \
            unsigned int wide_value = (unsigned int)(value);                                                             \
            unsigned int wide_old;                                                                                       \
            unsigned int wide_new;                                                                                       \
            unsigned int status;                                                                                         \
            __asm__ volatile(                                                                                            \
                "dmb ish\n\t"                                                                                            \
                "1: ldaxrb %w[wide_old], %[obj]\n\t" instruction " %w[wide_new], %w[wide_old], "                         \
                "%w[wide_value]\n\t"                                                                                     \
                "stlxrb %w[status], %w[wide_new], %[obj]\n\t"                                                            \
                "cbnz %w[status], 1b\n\t"                                                                                \
                "dmb ish\n\t"                                                                                            \
                : [wide_old] "=&r"(wide_old), [wide_new] "=&r"(wide_new), [status] "=&r"(status), [obj] "+Q"(obj->value) \
                : [wide_value] "r"(wide_value)                                                                           \
                : "cc", "memory");                                                                                       \
            return (type)wide_old;

        #define SN_ARM64_FETCH_2(type, obj, value, instruction)                                                          \
            unsigned int wide_value = (unsigned int)(value);                                                             \
            unsigned int wide_old;                                                                                       \
            unsigned int wide_new;                                                                                       \
            unsigned int status;                                                                                         \
            __asm__ volatile(                                                                                            \
                "dmb ish\n\t"                                                                                            \
                "1: ldaxrh %w[wide_old], %[obj]\n\t" instruction " %w[wide_new], %w[wide_old], "                         \
                "%w[wide_value]\n\t"                                                                                     \
                "stlxrh %w[status], %w[wide_new], %[obj]\n\t"                                                            \
                "cbnz %w[status], 1b\n\t"                                                                                \
                "dmb ish\n\t"                                                                                            \
                : [wide_old] "=&r"(wide_old), [wide_new] "=&r"(wide_new), [status] "=&r"(status), [obj] "+Q"(obj->value) \
                : [wide_value] "r"(wide_value)                                                                           \
                : "cc", "memory");                                                                                       \
            return (type)wide_old;

        #define SN_ARM64_FETCH_4(type, obj, value, instruction)                                                          \
            unsigned int wide_value = (unsigned int)(value);                                                             \
            unsigned int wide_old;                                                                                       \
            unsigned int wide_new;                                                                                       \
            unsigned int status;                                                                                         \
            __asm__ volatile(                                                                                            \
                "dmb ish\n\t"                                                                                            \
                "1: ldaxr %w[wide_old], %[obj]\n\t" instruction " %w[wide_new], %w[wide_old], "                          \
                "%w[wide_value]\n\t"                                                                                     \
                "stlxr %w[status], %w[wide_new], %[obj]\n\t"                                                             \
                "cbnz %w[status], 1b\n\t"                                                                                \
                "dmb ish\n\t"                                                                                            \
                : [wide_old] "=&r"(wide_old), [wide_new] "=&r"(wide_new), [status] "=&r"(status), [obj] "+Q"(obj->value) \
                : [wide_value] "r"(wide_value)                                                                           \
                : "cc", "memory");                                                                                       \
            return (type)wide_old;

        #define SN_ARM64_FETCH_8(type, obj, value, instruction)                                                          \
            uint64_t wide_value = (uint64_t)(value);                                                                     \
            uint64_t wide_old;                                                                                           \
            uint64_t wide_new;                                                                                           \
            unsigned int status;                                                                                         \
            __asm__ volatile(                                                                                            \
                "dmb ish\n\t"                                                                                            \
                "1: ldaxr %[wide_old], %[obj]\n\t" instruction " %[wide_new], %[wide_old], "                             \
                "%[wide_value]\n\t"                                                                                      \
                "stlxr %w[status], %[wide_new], %[obj]\n\t"                                                              \
                "cbnz %w[status], 1b\n\t"                                                                                \
                "dmb ish\n\t"                                                                                            \
                : [wide_old] "=&r"(wide_old), [wide_new] "=&r"(wide_new), [status] "=&r"(status), [obj] "+Q"(obj->value) \
                : [wide_value] "r"(wide_value)                                                                           \
                : "cc", "memory");                                                                                       \
            return (type)wide_old;

        #define SN_ARM64_DEFINE_FETCH(type, operation, instruction)                                \
            type SN_GET_ATOMIC_FUNCTION(fetch_##operation, type)(                                  \
                volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
                SN_UNUSED(memory_order);                                                           \
                if (sizeof(type) == 1) {                                                           \
                    SN_ARM64_FETCH_1(type, obj, value, instruction);                               \
                }                                                                                  \
                if (sizeof(type) == 2) {                                                           \
                    SN_ARM64_FETCH_2(type, obj, value, instruction);                               \
                }                                                                                  \
                if (sizeof(type) == 4) {                                                           \
                    SN_ARM64_FETCH_4(type, obj, value, instruction);                               \
                }                                                                                  \
                if (sizeof(type) == 8) {                                                           \
                    SN_ARM64_FETCH_8(type, obj, value, instruction);                               \
                }                                                                                  \
            }

        #define DEFINE_ATOMIC_FETCH_ADD(type) SN_ARM64_DEFINE_FETCH(type, add, "add")

        #define DEFINE_ATOMIC_FETCH_SUB(type) SN_ARM64_DEFINE_FETCH(type, sub, "sub")

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
    PRE_ATOMIC_RMW_FENCE(memory_order);
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
    POST_ATOMIC_RMW_FENCE(memory_order);
    return ret;
}

void sn_atomic_flag_clear_explicit(volatile sn_atomic_flag *obj, SnMemoryOrder memory_order) {
    PRE_ATOMIC_RMW_FENCE(memory_order);
    bool reset = false;
    /* A store release, not a plain store, so clearing is a read modify write
       like every other RMW here and cannot be reordered against a concurrent
       test and set. */
    __asm__ volatile("stlrb %w[value], %[flag]"
                     : [flag] "+Q"(obj->flag)
                     : [value] "r"(reset)
                     : "memory");
    POST_ATOMIC_RMW_FENCE(memory_order);
}

bool sn_atomic_flag_load_explicit(volatile sn_atomic_flag *obj, SnMemoryOrder memory_order) {
    PRE_ATOMIC_LOAD_FENCE(memory_order);
    bool ret;
    /* The memory clobber is what stops the load being hoisted out of a loop,
       a byte wide load has no way to tell the compiler it is not reusable. */
    __asm__ volatile("ldrb %w[value], %[flag]"
                     : [value] "=r"(ret)
                     : [flag] "Q"(obj->flag)
                     : "memory");
    POST_ATOMIC_LOAD_FENCE(memory_order);
    return ret;
}

        #include "../atomics_shared.h"

    #endif  // !SN_COMPILER_MSVC

#endif  // SN_ARCH_ARM64

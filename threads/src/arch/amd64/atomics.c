#include "snthreads/atomics.h"

#ifdef SN_ARCH_AMD64

    #if defined(SN_COMPILER_MSVC)

        #include "../atomics_shared.h"

    #else  // GCC/Clang

        #define LFENCE __asm__ volatile("lfence" ::: "memory")
        #define SFENCE __asm__ volatile("sfence" ::: "memory")
        #define MFENCE __asm__ volatile("mfence" ::: "memory")

        #define PRE_ATOMIC_LOAD_FENCE(memory_order)                      \
            do                                                           \
                if (memory_order == SN_MEMORY_ORDER_TOTAL_ORDER) MFENCE; \
            while (0)

        #define POST_ATOMIC_LOAD_FENCE(memory_order)                                                        \
            do                                                                                              \
                if (memory_order == SN_MEMORY_ORDER_ACQUIRE || memory_order == SN_MEMORY_ORDER_TOTAL_ORDER) \
                    LFENCE;                                                                                 \
            while (0)

        #define PRE_ATOMIC_STORE_FENCE(memory_order)                                                        \
            do                                                                                              \
                if (memory_order == SN_MEMORY_ORDER_RELEASE || memory_order == SN_MEMORY_ORDER_TOTAL_ORDER) \
                    SFENCE;                                                                                 \
            while (0)

        #define POST_ATOMIC_STORE_FENCE(memory_order)                    \
            do                                                           \
                if (memory_order == SN_MEMORY_ORDER_TOTAL_ORDER) MFENCE; \
            while (0)

        #define PRE_ATOMIC_RMW_FENCE(memory_order)                                                      \
            do                                                                                          \
                if (memory_order == SN_MEMORY_ORDER_RELEASE || memory_order == SN_MEMORY_ORDER_ACQ_REL) \
                    SFENCE;                                                                             \
                else if (memory_order == SN_MEMORY_ORDER_TOTAL_ORDER) MFENCE;                           \
            while (0)

        #define POST_ATOMIC_RMW_FENCE(memory_order)                                                     \
            do                                                                                          \
                if (memory_order == SN_MEMORY_ORDER_ACQUIRE || memory_order == SN_MEMORY_ORDER_ACQ_REL) \
                    LFENCE;                                                                             \
                else if (memory_order == SN_MEMORY_ORDER_TOTAL_ORDER) MFENCE;                           \
            while (0)

        #define DEFINE_ATOMIC_LOAD(type)                                                     \
            type SN_GET_ATOMIC_FUNCTION(load, type)(                                         \
                const volatile SN_GET_ATOMIC_TYPE(type) * obj, SnMemoryOrder memory_order) { \
                PRE_ATOMIC_LOAD_FENCE(memory_order);                                         \
                type value;                                                                  \
                __asm__ volatile("mov %[obj], %[value]"                                      \
                                 : [value] "=r"(value)                                       \
                                 : [obj] "rm"(obj->value));                                  \
                POST_ATOMIC_LOAD_FENCE(memory_order);                                        \
                return value;                                                                \
            }

        #define DEFINE_ATOMIC_STORE(type)                                                          \
            void SN_GET_ATOMIC_FUNCTION(store, type)(                                              \
                volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
                PRE_ATOMIC_STORE_FENCE(memory_order);                                              \
                __asm__ volatile("mov %[value], %[obj]"                                            \
                                 : [obj] "=m"(obj->value)                                          \
                                 : [value] "ir"(value)                                             \
                                 : "memory");                                                      \
                POST_ATOMIC_STORE_FENCE(memory_order);                                             \
            }

        #define DEFINE_ATOMIC_EXCHANGE(type)                                                       \
            type SN_GET_ATOMIC_FUNCTION(exchange, type)(                                           \
                volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
                SN_UNUSED(memory_order);                                                           \
                __asm__ volatile("lock xchg %[obj], %[value]"                                      \
                                 : [obj] "+m"(obj->value), [value] "+r"(value)                     \
                                 :                                                                 \
                                 : "memory");                                                      \
                return value;                                                                      \
            }

        /* cmpxchg always uses rax as its implicit accumulator, so the width of
           the object has to pick the register. Using rax unconditionally would
           read and write 8 bytes of a narrower object, which for an int32_t in
           the caller's frame means 4 bytes of stack past the end of it. The
           8 bit form also insists the incoming value arrives in cl. */
        #define SN_AMD64_CMPXCHG(accumulator, constraint)                                 \
            __asm__ volatile(                                                             \
                "mov %[expect], %%" accumulator "\n\t"                                    \
                "lock cmpxchg %[value], %[obj]\n\t"                                       \
                "mov %%" accumulator ", %[expect]\n\t"                                    \
                "sete %[swapped]"                                                         \
                : [expect] "+m"(*expect), [obj] "+m"(obj->value), [swapped] "=q"(swapped) \
                : [value] constraint(value)                                               \
                : "rax", "cc", "memory");

        #define DEFINE_ATOMIC_COMPARE_EXCHANGE(type)                                \
            bool SN_GET_ATOMIC_FUNCTION(compare_exchange, type)(                    \
                volatile SN_GET_ATOMIC_TYPE(type) * obj, type * expect, type value, \
                SnMemoryOrder success, SnMemoryOrder fail) {                        \
                SN_UNUSED(success);                                                 \
                SN_UNUSED(fail);                                                    \
                bool swapped;                                                       \
                if (sizeof(type) == 1) {                                            \
                    SN_AMD64_CMPXCHG("al", "c");                                    \
                } else if (sizeof(type) == 2) {                                     \
                    SN_AMD64_CMPXCHG("ax", "r");                                    \
                } else if (sizeof(type) == 4) {                                     \
                    SN_AMD64_CMPXCHG("eax", "r");                                   \
                } else {                                                            \
                    SN_AMD64_CMPXCHG("rax", "r");                                   \
                }                                                                   \
                return swapped;                                                     \
            }

        #define DEFINE_ATOMIC_FETCH_ADD(type)                                                      \
            type SN_GET_ATOMIC_FUNCTION(fetch_add, type)(                                          \
                volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
                SN_UNUSED(memory_order);                                                           \
                __asm__ volatile("lock xadd %[value], %[obj]"                                      \
                                 : [value] "+r"(value), [obj] "+m"(obj->value)                     \
                                 :                                                                 \
                                 : "cc", "memory");                                                \
                return value;                                                                      \
            }

        #define DEFINE_ATOMIC_FETCH_SUB(type)                                                      \
            type SN_GET_ATOMIC_FUNCTION(fetch_sub, type)(                                          \
                volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
                SN_UNUSED(memory_order);                                                           \
                __asm__ volatile("neg %[value]\n\t"                                                \
                                 "lock xadd %[value], %[obj]"                                      \
                                 : [value] "+r"(value), [obj] "+m"(obj->value)                     \
                                 :                                                                 \
                                 : "cc", "memory");                                                \
                return value;                                                                      \
            }

void sn_memory_fence(SnMemoryOrder fence) {
    switch (fence) {
        case SN_MEMORY_ORDER_NONE:
            break;
        case SN_MEMORY_ORDER_ACQUIRE:
            LFENCE;
            break;
        case SN_MEMORY_ORDER_RELEASE:
            SFENCE;
            break;
        case SN_MEMORY_ORDER_ACQ_REL:
        case SN_MEMORY_ORDER_TOTAL_ORDER:
            MFENCE;
            break;
    }
}

bool sn_atomic_flag_test_and_set_explicit(volatile sn_atomic_flag *obj, SnMemoryOrder memory_order) {
    PRE_ATOMIC_RMW_FENCE(memory_order);
    bool ret = true;
    __asm__ volatile("lock xchg %[flag], %[value]"
                     : [flag] "+m"(obj->flag), [value] "+r"(ret)
                     :
                     : "memory");
    POST_ATOMIC_RMW_FENCE(memory_order);
    return ret;
}

void sn_atomic_flag_clear_explicit(volatile sn_atomic_flag *obj, SnMemoryOrder memory_order) {
    PRE_ATOMIC_RMW_FENCE(memory_order);
    bool reset = false;
    /* An xchg, not a plain store, so clearing is a read modify write like
       every other RMW here and cannot be reordered against a concurrent
       test and set. */
    __asm__ volatile("xchg %[value], %[flag]"
                     : [flag] "+m"(obj->flag), [value] "+r"(reset)
                     :
                     : "memory");
    POST_ATOMIC_RMW_FENCE(memory_order);
}

bool sn_atomic_flag_load_explicit(volatile sn_atomic_flag *obj, SnMemoryOrder memory_order) {
    PRE_ATOMIC_LOAD_FENCE(memory_order);
    bool ret;
    /* The memory clobber is what stops the load being hoisted out of a loop,
       a byte wide load has no way to tell the compiler it is not reusable. */
    __asm__ volatile("mov %[flag], %[value]\n\t"
                     : [value] "=r"(ret)
                     : [flag] "m"(obj->flag)
                     : "memory");
    POST_ATOMIC_LOAD_FENCE(memory_order);
    return ret;
}

        #include "../atomics_shared.h"

    #endif  // defined(SN_COMPILER_MSVC)

#endif  // SN_ARCH_AMD64

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
                __asm__ volatile("mov%z1 %[value], %[obj]"                                         \
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

        #define DEFINE_ATOMIC_COMPARE_EXCHANGE(type)                                \
            bool SN_GET_ATOMIC_FUNCTION(compare_exchange, type)(                    \
                volatile SN_GET_ATOMIC_TYPE(type) * obj, type * expect, type value, \
                SnMemoryOrder success, SnMemoryOrder fail) {                        \
                SN_UNUSED(success);                                                 \
                SN_UNUSED(fail);                                                    \
                __asm__ goto(                                                       \
                    "mov %[expect], %%rax\n\t"                                      \
                    "lock cmpxchg %[value], %[obj]\n\t"                             \
                    "mov %%rax, %[expect]\n\t"                                      \
                    "jne %l[not_equal]"                                             \
                    : [expect] "+m"(*expect), [obj] "+m"(obj->value)                \
                    : [value] "ir"(value)                                           \
                    : "rax", "cc", "memory"                                         \
                    : not_equal);                                                   \
                return true;                                                        \
            not_equal:                                                              \
                return false;                                                       \
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
    SN_UNUSED(memory_order);
    bool ret = true;
    __asm__ volatile("lock xchg %[flag], %[value]"
                     : [flag] "+m"(obj->flag), [value] "+r"(ret)
                     :
                     : "memory");
    return ret;
}

void sn_atomic_flag_clear_explicit(volatile sn_atomic_flag *obj, SnMemoryOrder memory_order) {
    PRE_ATOMIC_RMW_FENCE(memory_order);
    bool reset = false;
    __asm__ volatile("movb %[value], %[flag]"
                     : [flag] "=m"(obj->flag)
                     : [value] "ir"(reset)
                     : "memory");
    POST_ATOMIC_RMW_FENCE(memory_order);
}

bool sn_atomic_flag_load_explicit(volatile sn_atomic_flag *obj, SnMemoryOrder memory_order) {
    PRE_ATOMIC_LOAD_FENCE(memory_order);
    bool ret;
    __asm__ volatile("mov %[flag], %[value]\n\t" : [value] "=r"(ret) : [flag] "m"(obj->flag));
    POST_ATOMIC_LOAD_FENCE(memory_order);
    return ret;
}

        #include "../atomics_shared.h"

    #endif  // defined(SN_COMPILER_MSVC)

#endif  // SN_ARCH_AMD64

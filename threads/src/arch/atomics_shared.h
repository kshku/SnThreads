#if defined(SN_COMPILER_MSVC)

    #define DEFINE_ATOMIC_SFL_LOAD(type)                                                 \
        type SN_GET_ATOMIC_FUNCTION(load, type)(                                         \
            const volatile SN_GET_ATOMIC_TYPE(type) * obj, SnMemoryOrder memory_order) { \
            switch (sizeof(type)) {                                                      \
                case 1:                                                                  \
                    return (type)SN_GET_ATOMIC_FUNCTION(load, int8_t)(                   \
                        (SN_GET_ATOMIC_TYPE(int8_t) *)obj, memory_order);                \
                case 2:                                                                  \
                    return (type)SN_GET_ATOMIC_FUNCTION(load, int16_t)(                  \
                        (SN_GET_ATOMIC_TYPE(int16_t) *)obj, memory_order);               \
                case 4:                                                                  \
                    return (type)SN_GET_ATOMIC_FUNCTION(load, int32_t)(                  \
                        (SN_GET_ATOMIC_TYPE(int32_t) *)obj, memory_order);               \
                case 8:                                                                  \
                    return (type)SN_GET_ATOMIC_FUNCTION(load, int64_t)(                  \
                        (SN_GET_ATOMIC_TYPE(int64_t) *)obj, memory_order);               \
                default:                                                                 \
                    return (type)0;                                                      \
            }                                                                            \
        }

    #define DEFINE_ATOMIC_UFL_LOAD(type)                                                 \
        type SN_GET_ATOMIC_FUNCTION(load, type)(                                         \
            const volatile SN_GET_ATOMIC_TYPE(type) * obj, SnMemoryOrder memory_order) { \
            switch (sizeof(type)) {                                                      \
                case 1:                                                                  \
                    return (type)SN_GET_ATOMIC_FUNCTION(load, uint8_t)(                  \
                        (SN_GET_ATOMIC_TYPE(uint8_t) *)obj, memory_order);               \
                case 2:                                                                  \
                    return (type)SN_GET_ATOMIC_FUNCTION(load, uint16_t)(                 \
                        (SN_GET_ATOMIC_TYPE(uint16_t) *)obj, memory_order);              \
                case 4:                                                                  \
                    return (type)SN_GET_ATOMIC_FUNCTION(load, uint32_t)(                 \
                        (SN_GET_ATOMIC_TYPE(uint32_t) *)obj, memory_order);              \
                case 8:                                                                  \
                    return (type)SN_GET_ATOMIC_FUNCTION(load, uint64_t)(                 \
                        (SN_GET_ATOMIC_TYPE(uint64_t) *)obj, memory_order);              \
                default:                                                                 \
                    return (type)0;                                                      \
            }                                                                            \
        }

    #define DEFINE_ATOMIC_SFL_STORE(type)                                                      \
        void SN_GET_ATOMIC_FUNCTION(store, type)(                                              \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
            switch (sizeof(type)) {                                                            \
                case 1:                                                                        \
                    SN_GET_ATOMIC_FUNCTION(store, int8_t)(                                     \
                        (SN_GET_ATOMIC_TYPE(int8_t) *)obj, (int8_t)value, memory_order);       \
                    break;                                                                     \
                case 2:                                                                        \
                    SN_GET_ATOMIC_FUNCTION(store, int16_t)(                                    \
                        (SN_GET_ATOMIC_TYPE(int16_t) *)obj, (int16_t)value, memory_order);     \
                    break;                                                                     \
                case 4:                                                                        \
                    SN_GET_ATOMIC_FUNCTION(store, int32_t)(                                    \
                        (SN_GET_ATOMIC_TYPE(int32_t) *)obj, (int32_t)value, memory_order);     \
                    break;                                                                     \
                case 8:                                                                        \
                    SN_GET_ATOMIC_FUNCTION(store, int64_t)(                                    \
                        (SN_GET_ATOMIC_TYPE(int64_t) *)obj, (int64_t)value, memory_order);     \
                    break;                                                                     \
                default:                                                                       \
                    break;                                                                     \
            }                                                                                  \
        }

    #define DEFINE_ATOMIC_UFL_STORE(type)                                                      \
        void SN_GET_ATOMIC_FUNCTION(store, type)(                                              \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
            switch (sizeof(type)) {                                                            \
                case 1:                                                                        \
                    SN_GET_ATOMIC_FUNCTION(store, uint8_t)(                                    \
                        (SN_GET_ATOMIC_TYPE(uint8_t) *)obj, (uint8_t)value, memory_order);     \
                    break;                                                                     \
                case 2:                                                                        \
                    SN_GET_ATOMIC_FUNCTION(store, uint16_t)(                                   \
                        (SN_GET_ATOMIC_TYPE(uint16_t) *)obj, (uint16_t)value, memory_order);   \
                    break;                                                                     \
                case 4:                                                                        \
                    SN_GET_ATOMIC_FUNCTION(store, uint32_t)(                                   \
                        (SN_GET_ATOMIC_TYPE(uint32_t) *)obj, (uint32_t)value, memory_order);   \
                    break;                                                                     \
                case 8:                                                                        \
                    SN_GET_ATOMIC_FUNCTION(store, uint64_t)(                                   \
                        (SN_GET_ATOMIC_TYPE(uint64_t) *)obj, (uint64_t)value, memory_order);   \
                    break;                                                                     \
                default:                                                                       \
                    break;                                                                     \
            }                                                                                  \
        }

    #define DEFINE_ATOMIC_SFL_EXCHANGE(type)                                                   \
        type SN_GET_ATOMIC_FUNCTION(exchange, type)(                                           \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
            switch (sizeof(type)) {                                                            \
                case 1:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(exchange, int8_t)(                     \
                        (SN_GET_ATOMIC_TYPE(int8_t) *)obj, (int8_t)value, memory_order);       \
                case 2:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(exchange, int16_t)(                    \
                        (SN_GET_ATOMIC_TYPE(int16_t) *)obj, (int16_t)value, memory_order);     \
                case 4:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(exchange, int32_t)(                    \
                        (SN_GET_ATOMIC_TYPE(int32_t) *)obj, (int32_t)value, memory_order);     \
                case 8:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(exchange, int64_t)(                    \
                        (SN_GET_ATOMIC_TYPE(int64_t) *)obj, (int64_t)value, memory_order);     \
                default:                                                                       \
                    return (type)0;                                                            \
            }                                                                                  \
        }

    #define DEFINE_ATOMIC_UFL_EXCHANGE(type)                                                   \
        type SN_GET_ATOMIC_FUNCTION(exchange, type)(                                           \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
            switch (sizeof(type)) {                                                            \
                case 1:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(exchange, uint8_t)(                    \
                        (SN_GET_ATOMIC_TYPE(uint8_t) *)obj, (uint8_t)value, memory_order);     \
                case 2:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(exchange, uint16_t)(                   \
                        (SN_GET_ATOMIC_TYPE(uint16_t) *)obj, (uint16_t)value, memory_order);   \
                case 4:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(exchange, uint32_t)(                   \
                        (SN_GET_ATOMIC_TYPE(uint32_t) *)obj, (uint32_t)value, memory_order);   \
                case 8:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(exchange, uint64_t)(                   \
                        (SN_GET_ATOMIC_TYPE(uint64_t) *)obj, (uint64_t)value, memory_order);   \
                default:                                                                       \
                    return (type)0;                                                            \
            }                                                                                  \
        }

    #define DEFINE_ATOMIC_SFL_COMPARE_EXCHANGE(type)                                                           \
        bool SN_GET_ATOMIC_FUNCTION(compare_exchange, type)(                                                   \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type * expect, type value,                                \
            SnMemoryOrder success, SnMemoryOrder fail) {                                                       \
            switch (sizeof(type)) {                                                                            \
                case 1:                                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(compare_exchange, int8_t)(                             \
                        (SN_GET_ATOMIC_TYPE(int8_t) *)obj, (int8_t *)expect, (int8_t)value, success, fail);    \
                case 2:                                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(compare_exchange, int16_t)(                            \
                        (SN_GET_ATOMIC_TYPE(int16_t) *)obj, (int16_t *)expect, (int16_t)value, success, fail); \
                case 4:                                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(compare_exchange, int32_t)(                            \
                        (SN_GET_ATOMIC_TYPE(int32_t) *)obj, (int32_t *)expect, (int32_t)value, success, fail); \
                case 8:                                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(compare_exchange, int64_t)(                            \
                        (SN_GET_ATOMIC_TYPE(int64_t) *)obj, (int64_t *)expect, (int64_t)value, success, fail); \
                default:                                                                                       \
                    return (type)0;                                                                            \
            }                                                                                                  \
        }

    #define DEFINE_ATOMIC_UFL_COMPARE_EXCHANGE(type)                                                              \
        bool SN_GET_ATOMIC_FUNCTION(compare_exchange, type)(                                                      \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type * expect, type value,                                   \
            SnMemoryOrder success, SnMemoryOrder fail) {                                                          \
            switch (sizeof(type)) {                                                                               \
                case 1:                                                                                           \
                    return (type)SN_GET_ATOMIC_FUNCTION(compare_exchange, uint8_t)(                               \
                        (SN_GET_ATOMIC_TYPE(uint8_t) *)obj, (uint8_t *)expect, (uint8_t)value, success, fail);    \
                case 2:                                                                                           \
                    return (type)SN_GET_ATOMIC_FUNCTION(compare_exchange, uint16_t)(                              \
                        (SN_GET_ATOMIC_TYPE(uint16_t) *)obj, (uint16_t *)expect, (uint16_t)value, success, fail); \
                case 4:                                                                                           \
                    return (type)SN_GET_ATOMIC_FUNCTION(compare_exchange, uint32_t)(                              \
                        (SN_GET_ATOMIC_TYPE(uint32_t) *)obj, (uint32_t *)expect, (uint32_t)value, success, fail); \
                case 8:                                                                                           \
                    return (type)SN_GET_ATOMIC_FUNCTION(compare_exchange, uint64_t)(                              \
                        (SN_GET_ATOMIC_TYPE(uint64_t) *)obj, (uint64_t *)expect, (uint64_t)value, success, fail); \
                default:                                                                                          \
                    return (type)0;                                                                               \
            }                                                                                                     \
        }

    #define DEFINE_ATOMIC_SFL_FETCH_ADD(type)                                                  \
        type SN_GET_ATOMIC_FUNCTION(fetch_add, type)(                                          \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
            switch (sizeof(type)) {                                                            \
                case 1:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_add, int8_t)(                    \
                        (SN_GET_ATOMIC_TYPE(int8_t) *)obj, (int8_t)value, memory_order);       \
                case 2:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_add, int16_t)(                   \
                        (SN_GET_ATOMIC_TYPE(int16_t) *)obj, (int16_t)value, memory_order);     \
                case 4:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_add, int32_t)(                   \
                        (SN_GET_ATOMIC_TYPE(int32_t) *)obj, (int32_t)value, memory_order);     \
                case 8:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_add, int64_t)(                   \
                        (SN_GET_ATOMIC_TYPE(int64_t) *)obj, (int64_t)value, memory_order);     \
                default:                                                                       \
                    return (type)0;                                                            \
            }                                                                                  \
        }

    #define DEFINE_ATOMIC_UFL_FETCH_ADD(type)                                                  \
        type SN_GET_ATOMIC_FUNCTION(fetch_add, type)(                                          \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
            switch (sizeof(type)) {                                                            \
                case 1:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_add, uint8_t)(                   \
                        (SN_GET_ATOMIC_TYPE(uint8_t) *)obj, (uint8_t)value, memory_order);     \
                case 2:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_add, uint16_t)(                  \
                        (SN_GET_ATOMIC_TYPE(uint16_t) *)obj, (uint16_t)value, memory_order);   \
                case 4:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_add, uint32_t)(                  \
                        (SN_GET_ATOMIC_TYPE(uint32_t) *)obj, (uint32_t)value, memory_order);   \
                case 8:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_add, uint64_t)(                  \
                        (SN_GET_ATOMIC_TYPE(uint64_t) *)obj, (uint64_t)value, memory_order);   \
                default:                                                                       \
                    return (type)0;                                                            \
            }                                                                                  \
        }

    #define DEFINE_ATOMIC_SFL_FETCH_SUB(type)                                                  \
        type SN_GET_ATOMIC_FUNCTION(fetch_sub, type)(                                          \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
            switch (sizeof(type)) {                                                            \
                case 1:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_sub, int8_t)(                    \
                        (SN_GET_ATOMIC_TYPE(int8_t) *)obj, (int8_t)value, memory_order);       \
                case 2:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_sub, int16_t)(                   \
                        (SN_GET_ATOMIC_TYPE(int16_t) *)obj, (int16_t)value, memory_order);     \
                case 4:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_sub, int32_t)(                   \
                        (SN_GET_ATOMIC_TYPE(int32_t) *)obj, (int32_t)value, memory_order);     \
                case 8:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_sub, int64_t)(                   \
                        (SN_GET_ATOMIC_TYPE(int64_t) *)obj, (int64_t)value, memory_order);     \
                default:                                                                       \
                    return (type)0;                                                            \
            }                                                                                  \
        }

    #define DEFINE_ATOMIC_UFL_FETCH_SUB(type)                                                  \
        type SN_GET_ATOMIC_FUNCTION(fetch_sub, type)(                                          \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
            switch (sizeof(type)) {                                                            \
                case 1:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_sub, uint8_t)(                   \
                        (SN_GET_ATOMIC_TYPE(uint8_t) *)obj, (uint8_t)value, memory_order);     \
                case 2:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_sub, uint16_t)(                  \
                        (SN_GET_ATOMIC_TYPE(uint16_t) *)obj, (uint16_t)value, memory_order);   \
                case 4:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_sub, uint32_t)(                  \
                        (SN_GET_ATOMIC_TYPE(uint32_t) *)obj, (uint32_t)value, memory_order);   \
                case 8:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_sub, uint64_t)(                  \
                        (SN_GET_ATOMIC_TYPE(uint64_t) *)obj, (uint64_t)value, memory_order);   \
                default:                                                                       \
                    return (type)0;                                                            \
            }                                                                                  \
        }

    #define DEFINE_ATOMIC_SFL_FETCH_OR(type)                                                   \
        type SN_GET_ATOMIC_FUNCTION(fetch_or, type)(                                           \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
            switch (sizeof(type)) {                                                            \
                case 1:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_or, int8_t)(                     \
                        (SN_GET_ATOMIC_TYPE(int8_t) *)obj, (int8_t)value, memory_order);       \
                case 2:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_or, int16_t)(                    \
                        (SN_GET_ATOMIC_TYPE(int16_t) *)obj, (int16_t)value, memory_order);     \
                case 4:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_or, int32_t)(                    \
                        (SN_GET_ATOMIC_TYPE(int32_t) *)obj, (int32_t)value, memory_order);     \
                case 8:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_or, int64_t)(                    \
                        (SN_GET_ATOMIC_TYPE(int64_t) *)obj, (int64_t)value, memory_order);     \
                default:                                                                       \
                    return (type)0;                                                            \
            }                                                                                  \
        }

    #define DEFINE_ATOMIC_UFL_FETCH_OR(type)                                                   \
        type SN_GET_ATOMIC_FUNCTION(fetch_or, type)(                                           \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
            switch (sizeof(type)) {                                                            \
                case 1:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_or, uint8_t)(                    \
                        (SN_GET_ATOMIC_TYPE(uint8_t) *)obj, (uint8_t)value, memory_order);     \
                case 2:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_or, uint16_t)(                   \
                        (SN_GET_ATOMIC_TYPE(uint16_t) *)obj, (uint16_t)value, memory_order);   \
                case 4:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_or, uint32_t)(                   \
                        (SN_GET_ATOMIC_TYPE(uint32_t) *)obj, (uint32_t)value, memory_order);   \
                case 8:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_or, uint64_t)(                   \
                        (SN_GET_ATOMIC_TYPE(uint64_t) *)obj, (uint64_t)value, memory_order);   \
                default:                                                                       \
                    return (type)0;                                                            \
            }                                                                                  \
        }

    #define DEFINE_ATOMIC_SFL_FETCH_XOR(type)                                                  \
        type SN_GET_ATOMIC_FUNCTION(fetch_xor, type)(                                          \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
            switch (sizeof(type)) {                                                            \
                case 1:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_xor, int8_t)(                    \
                        (SN_GET_ATOMIC_TYPE(int8_t) *)obj, (int8_t)value, memory_order);       \
                case 2:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_xor, int16_t)(                   \
                        (SN_GET_ATOMIC_TYPE(int16_t) *)obj, (int16_t)value, memory_order);     \
                case 4:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_xor, int32_t)(                   \
                        (SN_GET_ATOMIC_TYPE(int32_t) *)obj, (int32_t)value, memory_order);     \
                case 8:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_xor, int64_t)(                   \
                        (SN_GET_ATOMIC_TYPE(int64_t) *)obj, (int64_t)value, memory_order);     \
                default:                                                                       \
                    return (type)0;                                                            \
            }                                                                                  \
        }

    #define DEFINE_ATOMIC_UFL_FETCH_XOR(type)                                                  \
        type SN_GET_ATOMIC_FUNCTION(fetch_xor, type)(                                          \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
            switch (sizeof(type)) {                                                            \
                case 1:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_xor, uint8_t)(                   \
                        (SN_GET_ATOMIC_TYPE(uint8_t) *)obj, (uint8_t)value, memory_order);     \
                case 2:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_xor, uint16_t)(                  \
                        (SN_GET_ATOMIC_TYPE(uint16_t) *)obj, (uint16_t)value, memory_order);   \
                case 4:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_xor, uint32_t)(                  \
                        (SN_GET_ATOMIC_TYPE(uint32_t) *)obj, (uint32_t)value, memory_order);   \
                case 8:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_xor, uint64_t)(                  \
                        (SN_GET_ATOMIC_TYPE(uint64_t) *)obj, (uint64_t)value, memory_order);   \
                default:                                                                       \
                    return (type)0;                                                            \
            }                                                                                  \
        }

    #define DEFINE_ATOMIC_SFL_FETCH_AND(type)                                                  \
        type SN_GET_ATOMIC_FUNCTION(fetch_and, type)(                                          \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
            switch (sizeof(type)) {                                                            \
                case 1:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_and, int8_t)(                    \
                        (SN_GET_ATOMIC_TYPE(int8_t) *)obj, (int8_t)value, memory_order);       \
                case 2:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_and, int16_t)(                   \
                        (SN_GET_ATOMIC_TYPE(int16_t) *)obj, (int16_t)value, memory_order);     \
                case 4:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_and, int32_t)(                   \
                        (SN_GET_ATOMIC_TYPE(int32_t) *)obj, (int32_t)value, memory_order);     \
                case 8:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_and, int64_t)(                   \
                        (SN_GET_ATOMIC_TYPE(int64_t) *)obj, (int64_t)value, memory_order);     \
                default:                                                                       \
                    return (type)0;                                                            \
            }                                                                                  \
        }

    #define DEFINE_ATOMIC_UFL_FETCH_AND(type)                                                  \
        type SN_GET_ATOMIC_FUNCTION(fetch_and, type)(                                          \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
            switch (sizeof(type)) {                                                            \
                case 1:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_and, uint8_t)(                   \
                        (SN_GET_ATOMIC_TYPE(uint8_t) *)obj, (uint8_t)value, memory_order);     \
                case 2:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_and, uint16_t)(                  \
                        (SN_GET_ATOMIC_TYPE(uint16_t) *)obj, (uint16_t)value, memory_order);   \
                case 4:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_and, uint32_t)(                  \
                        (SN_GET_ATOMIC_TYPE(uint32_t) *)obj, (uint32_t)value, memory_order);   \
                case 8:                                                                        \
                    return (type)SN_GET_ATOMIC_FUNCTION(fetch_and, uint64_t)(                  \
                        (SN_GET_ATOMIC_TYPE(uint64_t) *)obj, (uint64_t)value, memory_order);   \
                default:                                                                       \
                    return (type)0;                                                            \
            }                                                                                  \
        }

    #define DEFINE_ATOMIC_SFL_FUNCTIONS(type)    \
        DEFINE_ATOMIC_SFL_LOAD(type)             \
        DEFINE_ATOMIC_SFL_STORE(type)            \
        DEFINE_ATOMIC_SFL_EXCHANGE(type)         \
        DEFINE_ATOMIC_SFL_COMPARE_EXCHANGE(type) \
        DEFINE_ATOMIC_SFL_FETCH_ADD(type)        \
        DEFINE_ATOMIC_SFL_FETCH_SUB(type)        \
        DEFINE_ATOMIC_SFL_FETCH_OR(type)         \
        DEFINE_ATOMIC_SFL_FETCH_XOR(type)        \
        DEFINE_ATOMIC_SFL_FETCH_AND(type)

    #define DEFINE_ATOMIC_UFL_FUNCTIONS(type)    \
        DEFINE_ATOMIC_UFL_LOAD(type)             \
        DEFINE_ATOMIC_UFL_STORE(type)            \
        DEFINE_ATOMIC_UFL_EXCHANGE(type)         \
        DEFINE_ATOMIC_UFL_COMPARE_EXCHANGE(type) \
        DEFINE_ATOMIC_UFL_FETCH_ADD(type)        \
        DEFINE_ATOMIC_UFL_FETCH_SUB(type)        \
        DEFINE_ATOMIC_UFL_FETCH_OR(type)         \
        DEFINE_ATOMIC_UFL_FETCH_XOR(type)        \
        DEFINE_ATOMIC_UFL_FETCH_AND(type)

DEFINE_ATOMIC_SFL_FUNCTIONS(int_fast8_t);
DEFINE_ATOMIC_SFL_FUNCTIONS(int_fast16_t);
DEFINE_ATOMIC_SFL_FUNCTIONS(int_fast32_t);
DEFINE_ATOMIC_SFL_FUNCTIONS(int_fast64_t);

DEFINE_ATOMIC_SFL_FUNCTIONS(int_least8_t);
DEFINE_ATOMIC_SFL_FUNCTIONS(int_least16_t);
DEFINE_ATOMIC_SFL_FUNCTIONS(int_least32_t);
DEFINE_ATOMIC_SFL_FUNCTIONS(int_least64_t);

DEFINE_ATOMIC_UFL_FUNCTIONS(uint_fast8_t);
DEFINE_ATOMIC_UFL_FUNCTIONS(uint_fast16_t);
DEFINE_ATOMIC_UFL_FUNCTIONS(uint_fast32_t);
DEFINE_ATOMIC_UFL_FUNCTIONS(uint_fast64_t);

DEFINE_ATOMIC_UFL_FUNCTIONS(uint_least8_t);
DEFINE_ATOMIC_UFL_FUNCTIONS(uint_least16_t);
DEFINE_ATOMIC_UFL_FUNCTIONS(uint_least32_t);
DEFINE_ATOMIC_UFL_FUNCTIONS(uint_least64_t);

#else  // GCC/Clang shared patterns

    #define ATOMIC_OP_USING_CAS(type, obj, op, value, old_var)                        \
        do {                                                                          \
            type new_val;                                                             \
            old_var = SN_GET_ATOMIC_FUNCTION(load, type)(obj, SN_MEMORY_ORDER_NONE);  \
            do new_val = old_var op value;                                            \
            while (SN_GET_ATOMIC_FUNCTION(compare_exchange, type)(                    \
                obj, &old_var, new_val, SN_MEMORY_ORDER_NONE, SN_MEMORY_ORDER_NONE)); \
        } while (0)

    #define DEFINE_ATOMIC_FETCH_OR(type)                                                       \
        type SN_GET_ATOMIC_FUNCTION(fetch_or, type)(                                           \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
            PRE_ATOMIC_RMW_FENCE(memory_order);                                                \
            type old;                                                                          \
            ATOMIC_OP_USING_CAS(type, obj, |, value, old);                                     \
            POST_ATOMIC_RMW_FENCE(memory_order);                                               \
            return old;                                                                        \
        }

    #define DEFINE_ATOMIC_FETCH_XOR(type)                                                      \
        type SN_GET_ATOMIC_FUNCTION(fetch_xor, type)(                                          \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
            PRE_ATOMIC_RMW_FENCE(memory_order);                                                \
            type old;                                                                          \
            ATOMIC_OP_USING_CAS(type, obj, ^, value, old);                                     \
            POST_ATOMIC_RMW_FENCE(memory_order);                                               \
            return old;                                                                        \
        }

    #define DEFINE_ATOMIC_FETCH_AND(type)                                                      \
        type SN_GET_ATOMIC_FUNCTION(fetch_and, type)(                                          \
            volatile SN_GET_ATOMIC_TYPE(type) * obj, type value, SnMemoryOrder memory_order) { \
            PRE_ATOMIC_RMW_FENCE(memory_order);                                                \
            type old;                                                                          \
            ATOMIC_OP_USING_CAS(type, obj, &, value, old);                                     \
            POST_ATOMIC_RMW_FENCE(memory_order);                                               \
            return old;                                                                        \
        }

    #define DEFINE_ATOMIC_FUNCTIONS(type)    \
        DEFINE_ATOMIC_LOAD(type)             \
        DEFINE_ATOMIC_STORE(type)            \
        DEFINE_ATOMIC_EXCHANGE(type)         \
        DEFINE_ATOMIC_COMPARE_EXCHANGE(type) \
        DEFINE_ATOMIC_FETCH_ADD(type)        \
        DEFINE_ATOMIC_FETCH_SUB(type)        \
        DEFINE_ATOMIC_FETCH_OR(type)         \
        DEFINE_ATOMIC_FETCH_XOR(type)        \
        DEFINE_ATOMIC_FETCH_AND(type)

DEFINE_ATOMIC_FUNCTIONS(int8_t)
DEFINE_ATOMIC_FUNCTIONS(int16_t)
DEFINE_ATOMIC_FUNCTIONS(int32_t)
DEFINE_ATOMIC_FUNCTIONS(int64_t)

DEFINE_ATOMIC_FUNCTIONS(int_fast8_t)
DEFINE_ATOMIC_FUNCTIONS(int_fast16_t)
DEFINE_ATOMIC_FUNCTIONS(int_fast32_t)
DEFINE_ATOMIC_FUNCTIONS(int_fast64_t)

DEFINE_ATOMIC_FUNCTIONS(int_least8_t)
DEFINE_ATOMIC_FUNCTIONS(int_least16_t)
DEFINE_ATOMIC_FUNCTIONS(int_least32_t)
DEFINE_ATOMIC_FUNCTIONS(int_least64_t)

DEFINE_ATOMIC_FUNCTIONS(uint8_t)
DEFINE_ATOMIC_FUNCTIONS(uint16_t)
DEFINE_ATOMIC_FUNCTIONS(uint32_t)
DEFINE_ATOMIC_FUNCTIONS(uint64_t)

DEFINE_ATOMIC_FUNCTIONS(uint_fast8_t)
DEFINE_ATOMIC_FUNCTIONS(uint_fast16_t)
DEFINE_ATOMIC_FUNCTIONS(uint_fast32_t)
DEFINE_ATOMIC_FUNCTIONS(uint_fast64_t)

DEFINE_ATOMIC_FUNCTIONS(uint_least8_t)
DEFINE_ATOMIC_FUNCTIONS(uint_least16_t)
DEFINE_ATOMIC_FUNCTIONS(uint_least32_t)
DEFINE_ATOMIC_FUNCTIONS(uint_least64_t)

#endif

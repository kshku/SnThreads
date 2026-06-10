#pragma once

#include <sncore/api_common.h>
#if defined(SN_THREADS_STATIC)
    #define SN_THREADS_API
#elif defined(SN_EXPORT)
    #define SN_THREADS_API SN_API_HELPER_EXPORT
#else
    #define SN_THREADS_API SN_API_HELPER_IMPORT
#endif

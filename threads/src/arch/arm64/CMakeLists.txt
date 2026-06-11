set(SRCS
    atomics.c
)

if(MSVC AND CMAKE_SYSTEM_PROCESSOR MATCHES "ARM64|aarch64|AARCH64")
    list(APPEND SRCS atomics.asm)
    target_link_options(snthreads PRIVATE "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/../atomics.def")
endif()

target_sources(snthreads PRIVATE ${SRCS})

set(SRCS
    atomics.c
)

if(MSVC)
    set(ASM_SRCS
        atomics.asm
    )

    list(APPEND SRCS ${ASM_SRCS})

    target_link_options(snthreads PRIVATE "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/atomics.def")
endif()

target_sources(snthreads PRIVATE ${SRCS})


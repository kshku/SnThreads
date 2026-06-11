set(SRCS
    atomics.c
)

if(MSVC)
    set(ASM_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/atomics.asm"
    )

    list(APPEND SRCS ${ASM_SRCS})

    target_link_options(snthreads PRIVATE "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/atomics.def")
endif()

set(USE_ABS_SRCS)
foreach(src IN LISTS SRCS)
    if(IS_ABSOLUTE "${src}")
        list(APPEND USE_ABS_SRCS "${src}")
    else()
        list(APPEND USE_ABS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
    endif()
endforeach()
target_sources(snthreads PRIVATE ${USE_ABS_SRCS})


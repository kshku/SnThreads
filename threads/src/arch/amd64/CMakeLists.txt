set(SRCS
    atomics.c
)

if(MSVC)
    set(ASM_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/atomics.asm"
    )

    list(APPEND SRCS ${ASM_SRCS})

    target_link_options(snthreads PRIVATE "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/atomics.def")

    add_custom_command(TARGET snthreads PRE_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory "$(IntDir)src/arch/amd64"
        COMMENT "Creating MASM output directory"
    )
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


target_sources(snthreads PRIVATE atomics.c)

if(MSVC)
    set(ASM_SRC "${CMAKE_CURRENT_SOURCE_DIR}/atomics.asm")
    set(ASM_OBJ "${CMAKE_CURRENT_BINARY_DIR}/atomics.asm.obj")

    add_custom_command(
        OUTPUT "${ASM_OBJ}"
        COMMAND ml64.exe /c /nologo "/Fo${ASM_OBJ}" "/Ta${ASM_SRC}"
        MAIN_DEPENDENCY "${ASM_SRC}"
        COMMENT "Assembling atomics.asm"
    )

    target_sources(snthreads PRIVATE "${ASM_OBJ}")
    target_link_options(snthreads PRIVATE "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/atomics.def")
endif()


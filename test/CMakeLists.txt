set(SRCS
    test.c
)

add_executable(sn_threads_test)

target_sources(sn_threads_test PRIVATE ${SRCS})

target_link_libraries(sn_threads_test PRIVATE snthreads)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_custom_target(copy_dlls ALL
        COMMENT "Copy the dlls"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_RUNTIME_DLLS:sn_threads_test>" "$<TARGET_FILE_DIR:sn_threads_test>"
        COMMAND_EXPAND_LISTS
    )

    add_dependencies(copy_dlls sn_threads_test)
endif()


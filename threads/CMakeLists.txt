if(SN_THREADS_BUILD_SHARED)
    add_library(snthreads SHARED)
    target_compile_definitions(snthreads PRIVATE SN_EXPORT)
else()
    add_library(snthreads STATIC)
    target_compile_definitions(snthreads PUBLIC SN_THREADS_STATIC)
endif()

target_include_directories(snthreads PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(snthreads PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(snthreads PRIVATE sn_threads_configs)
target_link_libraries(snthreads PUBLIC sncore)

if(MSVC)
    add_custom_command(TARGET snthreads PRE_BUILD
        COMMAND "${CMAKE_COMMAND}" -E make_directory "$(IntDir)src/arch/amd64"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "$(IntDir)src/arch/arm64"
        COMMENT "Creating MASM output directories"
    )
endif()

add_subdirectory(src)

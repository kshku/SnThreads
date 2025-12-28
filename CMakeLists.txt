cmake_minimum_required(VERSION 3.15...3.31)

project(SnThreads VERSION 0.0.0 DESCRIPTION "Thread and synchronization abstraction library" LANGUAGES C)

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)

add_library(sn_threads_configs INTERFACE)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(sn_threads_configs INTERFACE SN_DEBUG)
endif()

set(gcc_clang_comp "$<COMPILE_LANG_AND_ID:C,Clang,GNU>")
set(msvc_comp "$<COMPILE_LANG_AND_ID:C,MSVC>")

target_compile_options(sn_threads_configs INTERFACE "$<${gcc_clang_comp}:-Wall;-Wextra;-Wformat;-Wuninitialized;-Wpedantic>")

target_compile_options(sn_threads_configs INTERFACE "$<${gcc_clang_comp}:-fvisibility=hidden>")

target_compile_definitions(sn_threads_configs INTERFACE
    SN_THREADS_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    SN_THREADS_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    SN_THREADS_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)

option(SN_THREADS_BUILD_SHARED "Build shared library" OFF)
option(SN_THREADS_BUILD_TEST "Build tests" OFF)

add_subdirectory(docs)
add_subdirectory(threads)

if(SN_THREADS_BUILD_TEST)
    add_subdirectory("test")
else()
    message(STATUS "Building test is disabled")
endif()

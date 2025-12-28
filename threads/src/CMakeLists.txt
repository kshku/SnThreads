set(HEADERFILES
    platform.h
    defines.h
    snthreads.h
    atomics.h
    spinlock.h
    mutex.h
    rwlock.h
    condvar.h
    semaphore.h
)

set(SRCS
)

set(SPECIFIC_SRCS
    snthreads.c
    mutex.c
    rwlock.c
    condvar.c
    semaphore.c
)

set(SUBDIRS
    win32
    pthread
)

set(INCLUDE_BASE "${PROJECT_SOURCE_DIR}/threads/include/snthreads")

list(TRANSFORM HEADERFILES PREPEND "${INCLUDE_BASE}/")

target_sources(snthreads PRIVATE ${HEADERFILES} ${SRCS})

foreach(dir IN LISTS SUBDIRS)
    foreach(src IN LISTS SPECIFIC_SRCS)
        target_sources(snthreads PRIVATE "${dir}/${src}")
    endforeach()
endforeach()

add_subdirectory(arch)

set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES circular_buffer.c)

if (CMAKE_C_COMPILER_ID MATCHES GNU)
    set (CMAKE_C_FLAGS "-Wall -pedantic -Wextra")
    set (CMAKE_C_FLAGS_DEBUG "-g")
    set (CMAKE_C_FLAGS_RELEASE "-Werror")
endif()

if (CMAKE_C_COMPILER_ID MATCHES CLANG)
    set (CMAKE_C_FLAGS "-Weverything -pedantic")
    set (CMAKE_C_FLAGS_DEBUG "-g")
    set (CMAKE_C_FLAGS_RELEASE "-Werror")
endif()

#add_executable(${BINARY}_run ${SOURCES})
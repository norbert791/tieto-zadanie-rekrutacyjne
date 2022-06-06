set(BINARY ${CMAKE_PROJECT_NAME}_run)

set(SOURCES main.c circular_buffer.c thread_parser.c thread_printer.c thread_reader.c proc_parser.c pcp_guard.c watchdog.c thread_watchdog.c thread_logger.c logger_payload.c)

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

add_executable(${BINARY} ${SOURCES})
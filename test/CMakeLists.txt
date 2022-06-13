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


enable_testing()

add_executable(circular_buffer_test ${PROJECT_SOURCE_DIR}/src/circular_buffer.c circular_buffer_test.c)
add_executable(proc_parser_test ${PROJECT_SOURCE_DIR}/src/proc_parser.c proc_parser_test.c)
add_executable(pcp_guard_test ${PROJECT_SOURCE_DIR}/src/pcp_guard.c pcp_guard_test.c)
add_executable(logger_payload_test ${PROJECT_SOURCE_DIR}/src/logger_payload.c logger_payload_test.c)
add_executable(watchdog_test ${PROJECT_SOURCE_DIR}/src/watchdog.c watchdog_test.c)

add_test(NAME circular_buffer_test  COMMAND circular_buffer_test)
add_test(NAME proc_parser_test  COMMAND proc_parser_test)
add_test(NAME pcp_guard_test COMMAND pcp_guard_test)
add_test(NAME logger_payload_test COMMAND  logger_payload_test)
add_test(NAME watchdog_test COMMAND watchdog_test)
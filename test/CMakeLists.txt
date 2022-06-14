enable_testing()

add_executable(circular_buffer_test ${PROJECT_SOURCE_DIR}/src/circular_buffer.c circular_buffer_test.c)
add_executable(proc_parser_test ${PROJECT_SOURCE_DIR}/src/proc_parser.c proc_parser_test.c)
add_executable(pcp_guard_test ${PROJECT_SOURCE_DIR}/src/pcp_guard.c pcp_guard_test.c)
add_executable(logger_payload_test ${PROJECT_SOURCE_DIR}/src/logger_payload.c logger_payload_test.c)
add_executable(watchdog_test ${PROJECT_SOURCE_DIR}/src/watchdog.c watchdog_test.c)

target_link_libraries(pcp_guard_test pthread)
target_link_libraries(watchdog_test pthread)
target_link_libraries(proc_parser_test PRIVATE m)
target_link_libraries(circular_buffer_test PRIVATE m)

add_test(NAME circular_buffer_test  COMMAND circular_buffer_test)
add_test(NAME proc_parser_test  COMMAND proc_parser_test)
add_test(NAME pcp_guard_test COMMAND pcp_guard_test)
add_test(NAME logger_payload_test COMMAND  logger_payload_test)
add_test(NAME watchdog_test COMMAND watchdog_test)
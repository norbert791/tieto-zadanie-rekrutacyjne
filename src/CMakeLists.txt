set(BINARY ${CMAKE_PROJECT_NAME}_run)

set(SOURCES main.c circular_buffer.c thread_parser.c thread_printer.c thread_reader.c proc_parser.c pcp_guard.c watchdog.c thread_watchdog.c thread_logger.c logger_payload.c)

add_executable(${BINARY} ${SOURCES})
target_link_libraries(${BINARY} "pthread")
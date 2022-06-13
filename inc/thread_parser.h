/**
 * @file thread_parser.h
 * @brief Parsing thread that uses char_buffer to receive bytes of raw data and
 * double_buffer to send % of core usage as double value. Each sequence of parsed data is
 * separated with THREAD_PARSER_END
 *
 */
#ifndef THREAD_PARSER_H
#define THREAD_PARSER_H

#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include "circular_buffer.h"
#include "watchdog.h"
#include "pcp_guard.h"

/**
 * Used for separating one sequence of output data from another
 */
#define THREAD_PARSER_END -1.0

typedef struct ThreadParserArguments {
    CircularBuffer* char_buffer;
    CircularBuffer* double_buffer;
    CircularBuffer* logger_buffer;
    PCPGuard* logger_buffer_guard;
    PCPGuard* char_buffer_guard;
    PCPGuard* double_buffer_guard;
    WatchdogControlUnit* control_unit;
    bool* is_working;
    pthread_mutex_t* working_mutex;

} ThreadParserArguments;

void* thread_parser(void* parser_arguments);

#endif

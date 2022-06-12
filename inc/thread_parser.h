/**
 * @file thread_parser.h
 * @brief Parsing thread that uses char_buffer to recieve raw data and
 * double buffer to send % of core usage
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

#define THREAD_PARSER_END -1.0

/**
 * @brief thread_parser arguments: circular char buffer for retrieving raw data
 * and circular double buffer for parsed data. PCP_Guards for synchronization.
 * 
 */
typedef struct thread_parser_arguments {
    Circular_Buffer* char_buffer;
    Circular_Buffer* double_buffer;
    Circular_Buffer* logger_buffer;
    PCP_Guard* logger_buffer_guard;
    PCP_Guard* char_buffer_guard;
    PCP_Guard* double_buffer_guard;
    Watchdog_Control_Unit* control_unit;
    bool* is_working;
    pthread_mutex_t* working_mutex;

} thread_parser_arguments;

void* thread_parser(void* parser_arguments);

#endif
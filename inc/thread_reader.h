/**
 * @file thread_reader.h
 * @brief Thread that reads raw data from /proc/stat and sends it.
 * 
 */
#ifndef THREAD_READER_H
#define THREAD_READER_H

#include <stdio.h>
#include <pthread.h>
#include "pcp_guard.h"
#include "watchdog.h"
#include "circular_buffer.h"

/**
 * @brief thread_reader arguments: circular char buffer for
 * sending raw data, pcp guard for synchronization and
 * FILE* for reading from proc/stat.
 * 
 */

/**
 * @brief thread_reader arguments: circular char buffer for
 * sending raw data, pcp guard for synchronization and
 * FILE* for reading from proc/stat.
 * 
 */

typedef struct thread_reader_arguments {
    PCP_Guard* char_buffer_guard;
    PCP_Guard* logger_buffer_guard;
    Circular_Buffer* char_buffer;
    Circular_Buffer* logger_buffer;
    Watchdog_Control_Unit* control_unit;
    FILE* input_file;
    bool* working;
    pthread_mutex_t* working_mutex;

} thread_reader_arguments;

void* thread_reader(void* reader_arguments);

#endif
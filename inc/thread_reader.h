/**
 * @file thread_reader.h
 * @brief Thread that reads raw data from input_file (pointing to /proc/stat) and sends it through
 * char_buffer
 * 
 */
#ifndef THREAD_READER_H
#define THREAD_READER_H

#include <stdio.h>
#include <pthread.h>
#include "pcp_guard.h"
#include "watchdog.h"
#include "circular_buffer.h"

typedef struct ThreadReaderArguments {
    PCPGuard* char_buffer_guard;
    PCPGuard* logger_buffer_guard;
    CircularBuffer* char_buffer;
    CircularBuffer* logger_buffer;
    WatchdogControlUnit* control_unit;
    FILE* input_file;
    bool* working;
    pthread_mutex_t* working_mutex;

} ThreadReaderArguments;

void* thread_reader(void* reader_arguments);

#endif

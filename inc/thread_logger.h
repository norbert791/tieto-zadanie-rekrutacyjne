#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include "pcp_guard.h"
#include "circular_buffer.h"

typedef enum thread_logger_message_type {
    WARNING,
    ERROR,
    DEBUG,
} thread_logger_message_type;

typedef struct thread_logger_args {

    PCP_Guard* guard;
    Circular_Buffer* logger_payload_pointer_buffer;
    bool* is_working;
    pthread_mutex_t* is_working_mutex;
    FILE* logger_output;

} thread_logger_args;

void* thread_logger(void* thread_logger_args);

#endif
#ifndef THREAD_PARSER_H
#define THREAD_PARSER_H

#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include "circular_buffer.h"
#include "pcp_guard.h"

typedef struct thread_parser_arguments {
    Circular_Buffer* char_buffer;
    Circular_Buffer* double_buffer;
    PCP_Guard* char_buffer_guard;
    PCP_Guard* double_buffer_guard;
    bool* is_working;
    pthread_mutex_t* working_mutex;

} thread_parser_arguments;

void* thread_parser(void* parser_arguments);

#endif
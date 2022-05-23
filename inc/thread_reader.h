#ifndef THREAD_READER_H
#define THREAD_READER_H

#include <stdio.h>
#include "pcp_guard.h"
#include "logger.h"

typedef struct thread_reader_arguments {
    PCP_Guard* Circular_Buffer_guard;
    Circular_Buffer* circular_buffer;
    FILE* input_file;
    bool* is_working;
    pthread_mutex_t* working_mutex;

} thread_reader_arguments;

void* thread_reader(void* args);



#endif
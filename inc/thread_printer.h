#ifndef THREAD_PRINTER_H
#define THREAD_PRINTER_H

#include <stdbool.h>
#include "pcp_guard.h"
#include "circular_buffer.h"


typedef struct thread_printer_arguments
{
    PCP_Guard* circular_buffer_guard;
    Circular_Buffer* circular_buffer;
    bool* is_working;
    pthread_mutex_t* working_mutex;
} thread_printer_arguments;


void* thread_printer(void* printer_argumets);

#endif
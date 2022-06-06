/**
 * @file thread_printer.h
 * @brief Thread that recieves parsed data and prints them.
 * 
 */
#ifndef THREAD_PRINTER_H
#define THREAD_PRINTER_H

#include <stdbool.h>
#include "pcp_guard.h"
#include "watchdog.h"
#include "circular_buffer.h"

/**
 * @brief thread_printer argumnets:
 * circular double buffer for retrieving parsed data
 * and guard for synchronization
 * 
 */
typedef struct thread_printer_arguments
{
    PCP_Guard* circular_buffer_guard;
    PCP_Guard* logger_buffer_guard;
    Circular_Buffer* circular_buffer;
    Circular_Buffer* logger_buffer;    
    Watchdog_Control_Unit* control_unit;
    bool* is_working;
    pthread_mutex_t* working_mutex;
} thread_printer_arguments;


void* thread_printer(void* printer_argumets);

#endif
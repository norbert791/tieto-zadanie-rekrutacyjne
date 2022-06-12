/**
 * @file thread_printer.h
 * @brief Thread that recieves parsed data through circular_buffer_guard
 * and prints it to terminal.
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
typedef struct ThreadPrinterArguments
{
    PCPGuard* circular_buffer_guard;
    PCPGuard* logger_buffer_guard;
    CircularBuffer* circular_buffer;
    CircularBuffer* logger_buffer;    
    WatchdogControlUnit* control_unit;
    bool* is_working;
    pthread_mutex_t* working_mutex;
} ThreadPrinterArguments;


void* thread_printer(void* printer_argumets);

#endif
/**
 * @file thread_watchdog.h   
 * @brief thread that uses watchdog to oversee threads
 * if one of the threads does not ping their control_unit, then
 * the entire program will be aborted.
 * 
 */
#ifndef THREAD_WATCHDOG_H
#define THREAD_WATCHDOG_H

#include <pthread.h>
#include "watchdog.h"

typedef struct ThreadWatchdogArguments {
    Watchdog* watchdog;
    bool* is_working;
    pthread_mutex_t* mutex;

} ThreadWatchdogArguments;


void* thread_watchdog(void* args);

#endif

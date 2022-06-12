/**
 * @file thread_watchdog.h   
 * @brief thread that uses watchdog to watchover threads
 * if one of the threads does not ping their control_unit, then
 * the entire program will be aborted.
 * 
 */
#ifndef THREADWATCH_DOG_H
#define THREADWATCH_DOG_H

#include <pthread.h>
#include "watchdog.h"

typedef struct ThreadWatchdogArguments {
    Watchdog* watchdog;
    bool* is_working;
    pthread_mutex_t* mutex;

} ThreadWatchdogArguments;


void* thread_watchdog(void* args);

#endif
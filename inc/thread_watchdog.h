#ifndef THREADWATCH_DOG_H
#define THREADWATCH_DOG_H

#include <pthread.h>
#include "watchdog.h"

typedef struct thread_watchdog_arguments {
    Watchdog* watchdog;
    bool* is_working;
    pthread_mutex_t* mutex;

} thread_watchdog_arguments;


void* thread_watchdog(void* args);

#endif
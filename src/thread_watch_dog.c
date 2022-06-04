#include <time.h>
#include <errno.h>
#include "thread_watchdog.h"


void* thread_watchdog(void* args) {

    if (args == NULL) {
        return NULL;
    }

    Watchdog* watchdog;
    bool* is_working;
    pthread_mutex_t* mutex;
    const struct timespec sleep_time = {.tv_nsec = 0, .tv_sec = 2};

    {
        thread_watchdog_arguments* temp = args;

        watchdog = temp->watchdog;
        is_working = temp->is_working;
        mutex = temp->mutex;
    }

    while(true) {
        pthread_mutex_lock(mutex);
        if (!*is_working) {
            pthread_mutex_unlock(mutex);
            break;
        }
        pthread_mutex_unlock(mutex);
        
        watchdog_mark_and_sweep(watchdog);
        if (nanosleep(&sleep_time, NULL) != 0) {
                errno = 0;
                perror("Sleep error\n");
            }
    }

    return NULL;
}
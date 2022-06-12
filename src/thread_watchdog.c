#include <time.h>
#include <errno.h>
#include <stdio.h>
#include "thread_watchdog.h"


void* thread_watchdog(void* args) {

    if (args == NULL) {
        return NULL;
    }

    Watchdog* watchdog = NULL;
    bool* is_working = NULL;
    pthread_mutex_t* mutex = NULL;
    const struct timespec sleep_time = {.tv_nsec = 0, .tv_sec = 2};

    {
        ThreadWatchdogArguments* temp = args;

        watchdog = temp->watchdog;
        is_working = temp->is_working;
        mutex = temp->mutex;
    }

    if (watchdog == NULL || is_working == NULL || mutex == NULL) {
        perror("Watchdog: one of arguments was NULL\n");
    }

    while(true) {
        pthread_mutex_lock(mutex);
        if (!*is_working) {
            pthread_mutex_unlock(mutex);
            break;
        }
        pthread_mutex_unlock(mutex);

        if (watchdog_check_puppies(watchdog)) {
            puts("Watchdog: One of the theads is not responding. Aborting...\n");
            abort();
            break;
        }
        if (nanosleep(&sleep_time, NULL) != 0) {
            errno = 0;
            perror("Sleep error\n");
        }
    }

    return NULL;
}

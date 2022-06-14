#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include "watchdog.h"

typedef struct Watchdog {

    size_t max_number_of_units;
    size_t number_of_units;
    WatchdogControlUnit* control_units[]; /*FAM*/

} Watchdog;

void watchdog_finish(WatchdogControlUnit* unit);
void watchdog_ping(WatchdogControlUnit* unit);
int watchdog_unit_lock(WatchdogControlUnit* unit);
int watchdog_unit_unlock(WatchdogControlUnit* unit);
int watchdog_unit_atomic_ping(WatchdogControlUnit* puppy);
int watchdog_unit_atomic_finish(WatchdogControlUnit* puppy);

Watchdog* watchdog_new(const size_t size) {
    if (size == 0) {
        return NULL;
    }

    Watchdog* result = calloc(1, sizeof(*result) + sizeof(*result->control_units) * size);

    if (result == NULL) {
        errno = 0;
        return NULL;
    }

    result->max_number_of_units = size;

    return result;
}

void watchdog_delete(Watchdog* const restrict watchdog) {
    free(watchdog);
}

bool watchdog_add_puppy(Watchdog* const restrict watchdog, WatchdogControlUnit* puppy) {
    if (watchdog == NULL || puppy == NULL) {
        return false;
    }

    if (watchdog->number_of_units == watchdog->max_number_of_units) {
        return false;
    }

    watchdog->number_of_units++;
    
    for (size_t i = 0; i < watchdog->max_number_of_units; i++) {
        if (watchdog->control_units[i] == NULL) {
            watchdog->control_units[i] = puppy;
            return true;
        }
    }

    return false;
}

void watchdog_remove_puppy(Watchdog* const restrict watchdog, WatchdogControlUnit* puppy) {
    
    if (watchdog == NULL || puppy == NULL) {
        return;
    }

    for (size_t i = 0; i < watchdog->max_number_of_units; i++) {
        if (watchdog->control_units[i] == puppy) {
            watchdog->control_units[i] = NULL;
            watchdog->number_of_units--;
            return;
        }
    }
}

bool watchdog_check_puppies(Watchdog* const watchdog) {
    bool sweep = false;
    for (size_t i = 0; i < watchdog->max_number_of_units; i++) {
        WatchdogControlUnit* puppy_ptr = watchdog->control_units[i];
        if (puppy_ptr != NULL) {
            watchdog_unit_lock(puppy_ptr);
            if (puppy_ptr->status == WATCHDOG_STATUS_UP) {
                puppy_ptr->status = WATCHDOG_STATUS_DOWN;
            }
            else if (puppy_ptr->status == WATCHDOG_STATUS_DOWN) {
               sweep = true;
               puppy_ptr->status = WATCHDOG_STATUS_ROUGE;
               watchdog_unit_unlock(puppy_ptr);
               continue;
            }
            watchdog_unit_unlock(puppy_ptr);
        }
    }
    return sweep;
}

void watchdog_clear(Watchdog* const watchdog) {
    for (size_t i = 0; i < watchdog->max_number_of_units; i++) {
        WatchdogControlUnit* puppy_ptr = watchdog->control_units[i];

        if (puppy_ptr->status == WATCHDOG_STATUS_ROUGE || puppy_ptr->status == WATCHDOG_STATUS_FINISHED) {
            watchdog->control_units[i] = NULL;
            puppy_ptr = NULL;
            watchdog->number_of_units--;
        }
    }
}

size_t watchdog_number_of_units(Watchdog* const watchdog) {
    if (watchdog == NULL) {
        return 0;
    }
    else {
        return watchdog->number_of_units;
    }
}

int watchdog_unit_init(WatchdogControlUnit* puppy, pthread_t thread_id) {

    if (puppy == NULL) {
        return -1;
    }
    puppy->status = WATCHDOG_STATUS_UP;
    puppy->thread_id = thread_id;
    return pthread_mutex_init(&puppy->unit_mutex, NULL);
}

int watchdog_unit_destroy(WatchdogControlUnit* puppy) {
    if (puppy == NULL) {
        return -1;
    }
    return pthread_mutex_destroy(&puppy->unit_mutex);
}

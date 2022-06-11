#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include "watchdog.h"

typedef struct Watchdog {

    size_t max_number_of_units;
    size_t number_of_units;
    Watchdog_Control_Unit* control_units[]; /*FAM*/

} Watchdog;

void watchdog_finish(Watchdog_Control_Unit* unit);
void watchdog_ping(Watchdog_Control_Unit* unit);
int watchdog_unit_lock(Watchdog_Control_Unit* unit);
int watchdog_unit_unlock(Watchdog_Control_Unit* unit);
int watchdog_unit_atomic_ping(Watchdog_Control_Unit* puppy);
int watchdog_unit_atomic_finish(Watchdog_Control_Unit* puppy);

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

void watchdog_delete(Watchdog* const restrict watch_dog) {
    free(watch_dog);
}

bool watchdog_add_puppy(Watchdog* const restrict watch_dog, Watchdog_Control_Unit* puppy) {
    if (watch_dog == NULL || puppy == NULL) {
        return false;
    }

    if (watch_dog->number_of_units == watch_dog->max_number_of_units) {
        return false;
    }

    watch_dog->number_of_units++;
    
    for (size_t i = 0; i < watch_dog->max_number_of_units; i++) {
        if (watch_dog->control_units[i] == NULL) {
            watch_dog->control_units[i] = puppy;
            return true;
        }
    }

    return false;
}

void watchdog_remove_puppy(Watchdog* const restrict watch_dog, Watchdog_Control_Unit* puppy) {
    
    if (watch_dog == NULL || puppy == NULL) {
        return;
    }

    for (size_t i = 0; i < watch_dog->max_number_of_units; i++) {
        if (watch_dog->control_units[i] == puppy) {
            watch_dog->control_units[i] = NULL;
            watch_dog->number_of_units--;
            return;
        }
    }
}

bool watchdog_check_puppies(Watchdog* const watch_dog) {
    bool sweep = false;
    for (size_t i = 0; i < watch_dog->max_number_of_units; i++) {
        Watchdog_Control_Unit* puppy_ptr = watch_dog->control_units[i];
        if (puppy_ptr != NULL) {
            watchdog_unit_lock(puppy_ptr);
            if (puppy_ptr->status == WATCH_DOG_STATUS_UP) {
                puppy_ptr->status = WATCH_DOG_STATUS_DOWN;
            }
            else if (puppy_ptr->status == WATCH_DOG_STATUS_DOWN) {
               sweep = true;
               puppy_ptr->status = WATCH_DOG_STATUS_ROUGE;
               watchdog_unit_unlock(puppy_ptr);
               continue;
            }
            watchdog_unit_unlock(puppy_ptr);
        }
    }
    return sweep;
}

void watchdog_clear(Watchdog* const watch_dog) {
    for (size_t i = 0; i < watch_dog->max_number_of_units; i++) {
        Watchdog_Control_Unit* puppy_ptr = watch_dog->control_units[i];

        if (puppy_ptr->status == WATCH_DOG_STATUS_ROUGE || puppy_ptr->status == WATCH_DOG_STATUS_FINISHED) {
            watch_dog->control_units[i] = NULL;
            puppy_ptr = NULL;
            watch_dog->number_of_units--;
        }
    }
}

size_t watchdog_number_of_units(Watchdog* const watch_dog) {
    if (watch_dog == NULL) {
        return 0;
    }
    else {
        return watch_dog->number_of_units;
    }
}
#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct Watchdog Watchdog;

typedef enum Watchdog_Status {
    WATCH_DOG_STATUS_UP,
    WATCH_DOG_STATUS_DOWN,
    WATCH_DOG_STATUS_FINISHED,
    WATCH_DOG_STATUS_CANCELED
} Watchdog_Status;

typedef struct Watchdog_Control_Unit {
    pthread_mutex_t unit_mutex;
    pthread_t thread_id;
    Watchdog_Status status;
} Watchdog_Control_Unit;

#define WATCHDOG_CONTROL_UNIT_INIT {PTHREAD_MUTEX_INITIALIZER, 0, WATCH_DOG_STATUS_UP}

inline void watchdog_ping(Watchdog_Control_Unit* unit) {
    unit->status = WATCH_DOG_STATUS_UP;
}

inline void watchdog_finish(Watchdog_Control_Unit* unit) {
    unit->status = WATCH_DOG_STATUS_FINISHED;
}

Watchdog* watchdog_new(size_t number_of_puppies);
void watchdog_delete(Watchdog* restrict watch_dog);
bool watchdog_add_puppy(Watchdog* restrict watch_dog, Watchdog_Control_Unit* restrict control_unit);
void watchdog_remove_puppy(Watchdog* watch_dog, Watchdog_Control_Unit* puppy);
void watchdog_mark_and_sweep(Watchdog* watch_dog);
void watchdog_clear(Watchdog* watch_dog);
size_t watchdog_number_of_units(Watchdog* watch_dog);

inline int watchdog_unit_lock(Watchdog_Control_Unit* unit) {
    return pthread_mutex_lock(&unit->unit_mutex);
}
inline int watchdog_unit_unlock(Watchdog_Control_Unit* unit) {
    return pthread_mutex_unlock(&unit->unit_mutex);
}

inline int watchdog_unit_atomic_ping(Watchdog_Control_Unit* puppy) {
    pthread_mutex_lock(&puppy->unit_mutex);
    puppy->status = WATCH_DOG_STATUS_UP;
    return pthread_mutex_unlock(&puppy->unit_mutex);
}

inline int watchdog_unit_atomic_finish(Watchdog_Control_Unit* puppy) {
    pthread_mutex_lock(&puppy->unit_mutex);
    puppy->status = WATCH_DOG_STATUS_FINISHED;
    return pthread_mutex_unlock(&puppy->unit_mutex);
}

#endif
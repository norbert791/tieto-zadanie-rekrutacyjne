/**
 * @file watchdog.h
 * @author supporting interface for watchdog thread
 * 
 */

#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>

/**
 * @brief type used for storing control units and operating on them.
 * 
 */
typedef struct Watchdog Watchdog;

typedef enum EWatchdogStatus {
    WATCH_DOG_STATUS_UP,
    WATCH_DOG_STATUS_DOWN,
    WATCH_DOG_STATUS_FINISHED,
    WATCH_DOG_STATUS_ROUGE
} EWatchdogStatus;

/**
 * @brief Control unit representing thread that should be overseen by the watchdog.
 * One thread shall use at most one control unit stored in at most one watchdog
 * 
 */
typedef struct WatchdogControlUnit {
    pthread_mutex_t unit_mutex;
    pthread_t thread_id;
    EWatchdogStatus status;
} WatchdogControlUnit;

#define WATCHDOG_CONTROL_UNIT_INIT {PTHREAD_MUTEX_INITIALIZER, 0, WATCH_DOG_STATUS_UP}

/**
 * @brief Set flag inside control unit to declare that the thread is still operating
 * 
 * @param unit pointer to valid control unit
 */
inline void watchdog_ping(WatchdogControlUnit* unit) {
    unit->status = WATCH_DOG_STATUS_UP;
}

/**
 * @brief Mark control unit as finished. Control unit marked as finished shall be ignored by watchdog.
 * 
 * @param unit 
 */
inline void watchdog_finish(WatchdogControlUnit* unit) {
    unit->status = WATCH_DOG_STATUS_FINISHED;
}

/**
 * @brief Create new watchdog
 * 
 * @param number_of_puppies max number of control units that newly created watchdog shall oversee
 * @return pointer to valid watchdog on success, NULL on failure.
 */
Watchdog* watchdog_new(size_t number_of_puppies);
/**
 * @brief create new watchdog that will be able to oversee number_of_puppies control units at most
 * @param pointer pointer to valid watchdog on success, NULL on failure or if number_of_puppies is equal to 0.
 */
void watchdog_delete(Watchdog* watch_dog);
/**
 * @brief insert control unit into the collection maintained by watchdog.
 * 
 * @param watch_dog pointer to watchdog whose collection shall be enlarged with new control unit
 * @param control_unit control unit that shall be overseen by watchdog
 * @return true iff inserting puppy was successful
 * @return false otherwise
 */
bool watchdog_add_puppy(Watchdog* restrict watch_dog, WatchdogControlUnit* restrict control_unit);
/**
 * @brief Remove control unit from watchdog pointed by watch_dog
 * 
 * @param watch_dog
 * @param puppy 
 */
void watchdog_remove_puppy(Watchdog* restrict watch_dog, WatchdogControlUnit* restrict puppy);
/**
 * @brief iterate ove the collection of control units stored inside watch_dog
 * 
 * @param puppy pointer to valid 
 * @param watch_dog pointer to valid watchdog
 */
bool watchdog_check_puppies(Watchdog* watch_dog);
/**
 * @brief remove all control units in collection that are no longer under
 * watchdog supervision (i.e. they are neither up nor down)
 *
 */
void watchdog_clear(Watchdog* watch_dog);

/**
 * @brief get number of watchdog units
 *
 * @param watch_dog pointer to valid watchdog
 * @return number of units stored in watchdog
 */
size_t watchdog_number_of_units(Watchdog* watch_dog);

/**
 * @brief wrapper for getting size of watch_dg
 * 
 * @param unit pointer to valid control unit
 * @return return value on pthread_mutex_lock
 */
inline int watchdog_unit_lock(WatchdogControlUnit* unit) {
    return pthread_mutex_lock(&unit->unit_mutex);
}

/**
 * @brief wrapper for calling watchdog_unit_unlock
 * 
 * @param unit pointer to valid watchdog
 * @return return code of pthread_cond_wait 
 */
inline int watchdog_unit_unlock(WatchdogControlUnit* unit) {
    return pthread_mutex_unlock(&unit->unit_mutex);
}

/**
 * @brief Wrapper of mutex lock/unlock and finish
 * @param puppy pointer to valid control unit
 * @return return value of pthread_muted unlock
 */
inline int watchdog_unit_atomic_ping(WatchdogControlUnit* puppy) {
    pthread_mutex_lock(&puppy->unit_mutex);
    puppy->status = WATCH_DOG_STATUS_UP;
    return pthread_mutex_unlock(&puppy->unit_mutex);
}

/**
 * @brief Wrapper for calling mutex lock
 * @param puppy pointer to valid WatchdogControlUnit
 * @return result of unlocking puppy's mutex
 */
inline int watchdog_unit_atomic_finish(WatchdogControlUnit* puppy) {
    pthread_mutex_lock(&puppy->unit_mutex);
    puppy->status = WATCH_DOG_STATUS_FINISHED;
    return pthread_mutex_unlock(&puppy->unit_mutex);
}

/**
 * @brief Initialize watchdog control unit.
 * 
 * @param puppy pointer to uninitialized WatchdogControlUnit 
 * @param thread_id pthread_t of thread that will be assigned to this control unit
 * @return return value of pthread_mutex_init used during initialization @see man pthread_mutex_init(3)
 * @return -1 if NULL was pussed as puppy
 */
int watchdog_unit_init(WatchdogControlUnit* puppy, pthread_t thread_id);

/**
 * @brief Destroy watchdog control unit
 * 
 * @param puppy pointer to WatchdogControlUnit that shall be destroyed
 * @return return value of pthread_mutex_destroy used during destruction procedure @see man pthread_mutex_destroy(3)
 * @return -1 if NULL was passed
 */
int watchdog_unit_destroy(WatchdogControlUnit* puppy);

#endif

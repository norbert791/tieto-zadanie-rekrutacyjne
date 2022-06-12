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
 * @brief type used for storing controll units and operating on them.
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
 * @brief Control unit representing thread that shold be overseen by the watchdog.
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
 * @brief Set flag inside controll unit to declare that the thread is still operating
 * 
 * @param unit pointer to valid controll unit
 */
inline void watchdog_ping(WatchdogControlUnit* unit) {
    unit->status = WATCH_DOG_STATUS_UP;
}

/**
 * @brief Mark controll unit as finished. Controll unit marked as finished shall be ignored by watchdog.
 * 
 * @param unit 
 */
inline void watchdog_finish(WatchdogControlUnit* unit) {
    unit->status = WATCH_DOG_STATUS_FINISHED;
}

/**
 * @brief Create new watchdog
 * 
 * @param number_of_puppies max number of controll_units that newly created watchdog shall oversee
 * @return pointer to valid watchdog on sucess, NULL on failure.
 */
Watchdog* watchdog_new(size_t number_of_puppies);
/**
 * @brief create new watchdog that will be able to oversee number_of_puppies controll units at most
 * @param pointer pointer to valid watchdog on success, NULL on failure or if number_of_puppies is equal to 0.
 */
void watchdog_delete(Watchdog* watch_dog);
/**
 * @brief insert controll_unit into the collection maintained by watchdog.
 * 
 * @param watch_dog pointer to watchdog whose collection shall be enlarged with new controll unit
 * @param control_unit controll unit that shall be overseen by watchdog
 * @return true 
 * @return false 
 */
bool watchdog_add_puppy(Watchdog* restrict watch_dog, WatchdogControlUnit* restrict control_unit);
/**
 * @brief Remove controll_unit from watchdog pointed by watch_dog
 * 
 * @param watch_dog 
 * @param puppy 
 */
void watchdog_remove_puppy(Watchdog* restrict watch_dog, WatchdogControlUnit* restrict puppy);
/**
 * @brief iterate ove the collection of controll units stored inside watch_dog
 * 
 * @param puppy pointer to valid 
 * @param watch_dog pointer to valid watchdog
 * @return true 
 * @return false 
 */
bool watchdog_check_puppies(Watchdog* watch_dog);
/**
 * @brief remove all unnecessary entraces in collection held by watchdog
 * 
  watch_dog 
 */
void watchdog_clear(Watchdog* watch_dog);
size_t watchdog_number_of_units(Watchdog* watch_dog);

/**
 * @brief wrappter for getting size of watch_dg
 * 
 * @param unit 
 * @return return value on pthread_mutex_lock
 */
inline int watchdog_unit_lock(WatchdogControlUnit* unit) {
    return pthread_mutex_lock(&unit->unit_mutex);
}

/**
 * @brief wrapper for calling watchdog_unit_unlock
 * 
 * @param unit pointer to valid waatchdog
 * @return retrun code of pthread_cont_wait 
 */
inline int watchdog_unit_unlock(WatchdogControlUnit* unit) {
    return pthread_mutex_unlock(&unit->unit_mutex);
}

/**
 * @brief Atomic wrapper of calling mutex lonck and finish
 * 
 * 
 * @param puppy 
 * @return int 
 */
inline int watchdog_unit_atomic_ping(WatchdogControlUnit* puppy) {
    pthread_mutex_lock(&puppy->unit_mutex);
    puppy->status = WATCH_DOG_STATUS_UP;
    return pthread_mutex_unlock(&puppy->unit_mutex);
}

/**
 * @brief Attomic wrapper for calling mutex lock
 * 
 * @param puppy pointer to valid 
 * @return int 
 */
inline int watchdog_unit_atomic_finish(WatchdogControlUnit* puppy) {
    pthread_mutex_lock(&puppy->unit_mutex);
    puppy->status = WATCH_DOG_STATUS_FINISHED;
    return pthread_mutex_unlock(&puppy->unit_mutex);
}

#endif
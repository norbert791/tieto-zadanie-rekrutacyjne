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
 * @brief type used for storing controll units and performin actions on them.
 * 
 */
typedef struct Watchdog Watchdog;

typedef enum Watchdog_Status {
    WATCH_DOG_STATUS_UP,
    WATCH_DOG_STATUS_DOWN,
    WATCH_DOG_STATUS_FINISHED,
    WATCH_DOG_STATUS_ROUGE
} Watchdog_Status;

/**
 * @brief Control unit representing thread that shold be overseen by the watchdog.
 * One thread shall use at most one control unit stored in at most one watchdog
 * 
 */
typedef struct Watchdog_Control_Unit {
    pthread_mutex_t unit_mutex;
    pthread_t thread_id;
    Watchdog_Status status;
} Watchdog_Control_Unit;

#define WATCHDOG_CONTROL_UNIT_INIT {PTHREAD_MUTEX_INITIALIZER, 0, WATCH_DOG_STATUS_UP}

/**
 * @brief Used flag inside controll unit to declare that the thread is still operating
 * 
 * @param unit pointer to valid controll unit
 */
inline void watchdog_ping(Watchdog_Control_Unit* unit) {
    unit->status = WATCH_DOG_STATUS_UP;
}

/**
 * @brief Mark controll unit as finished. Finished controll unit shall be ignored by certaing watchdog operation.
 * 
 * @param unit 
 */
inline void watchdog_finish(Watchdog_Control_Unit* unit) {
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
 * @brief create new watchdog that will be storing number_of_puppies at most
 * 
 * @param pointer to valid watchdog
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
bool watchdog_add_puppy(Watchdog* restrict watch_dog, Watchdog_Control_Unit* restrict control_unit);
/**
 * @brief Remove controll_unit from watchdog pointed by watch_dog
 * 
 * @param watch_dog 
 * @param puppy 
 */
void watchdog_remove_puppy(Watchdog* restrict watch_dog, Watchdog_Control_Unit* restrict puppy);
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
inline int watchdog_unit_lock(Watchdog_Control_Unit* unit) {
    return pthread_mutex_lock(&unit->unit_mutex);
}

/**
 * @brief wrapper for calling watchdog_unit_unlock
 * 
 * @param unit pointer to valid waatchdog
 * @return retrun code of pthread_cont_wait 
 */
inline int watchdog_unit_unlock(Watchdog_Control_Unit* unit) {
    return pthread_mutex_unlock(&unit->unit_mutex);
}

/**
 * @brief Atomic wrapper of calling mutex lonck and finish
 * 
 * 
 * @param puppy 
 * @return int 
 */
inline int watchdog_unit_atomic_ping(Watchdog_Control_Unit* puppy) {
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
inline int watchdog_unit_atomic_finish(Watchdog_Control_Unit* puppy) {
    pthread_mutex_lock(&puppy->unit_mutex);
    puppy->status = WATCH_DOG_STATUS_FINISHED;
    return pthread_mutex_unlock(&puppy->unit_mutex);
}

#endif
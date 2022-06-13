#include <pthread.h>
#include <assert.h>
#include "watchdog.h"


static void create_destroy_test();
static void add_remove_test();
static void clear_test();
static void ping_finish_test();
static void lock_unlock_test();
static void number_of_units_test();
static void create_unit_destroy_test();

void create_unit_destroy_test() {
    assert(watchdog_unit_init(NULL, 0) == -1);
    assert(watchdog_unit_destroy(NULL) == -1);
    
    WatchdogControlUnit unit;

    assert(watchdog_unit_init(&unit, 0) == 0);
    assert(watchdog_unit_destroy(&unit) == 0);
}

void ping_finish_test() {
    WatchdogControlUnit unit = {.status = WATCH_DOG_STATUS_DOWN, .thread_id = 0, .unit_mutex = PTHREAD_MUTEX_INITIALIZER};

    watchdog_ping(&unit);

    assert(unit.status == WATCH_DOG_STATUS_UP);

    watchdog_finish(&unit);

    assert(unit.status == WATCH_DOG_STATUS_FINISHED);

    unit.status = WATCH_DOG_STATUS_DOWN;

    watchdog_unit_atomic_ping(&unit);
    assert(unit.status == WATCH_DOG_STATUS_UP);
}

static void number_of_units_test() {
    Watchdog* dog = watchdog_new(1);
    WatchdogControlUnit unit = {.status = WATCH_DOG_STATUS_UP, .thread_id = 0, 
                            .unit_mutex = PTHREAD_MUTEX_INITIALIZER};

    assert(watchdog_number_of_units(dog) == 0);
    watchdog_add_puppy(dog, &unit);
    assert(watchdog_number_of_units(dog) == 1);
    watchdog_remove_puppy(dog, &unit);
    assert(watchdog_number_of_units(dog) == 0);
}

static void mark_and_sweep_test() {
    
    {
        WatchdogControlUnit unit = {.status = WATCH_DOG_STATUS_UP, .thread_id = 0, 
                            .unit_mutex = PTHREAD_MUTEX_INITIALIZER};
        Watchdog* dog = watchdog_new(1);
        watchdog_add_puppy(dog, &unit);

        watchdog_check_puppies(dog);
        
        assert(unit.status == WATCH_DOG_STATUS_DOWN);
        
        watchdog_delete(dog);
    }

    {
        WatchdogControlUnit unit = {.status = WATCH_DOG_STATUS_FINISHED, .thread_id = 0, 
                            .unit_mutex = PTHREAD_MUTEX_INITIALIZER};
        Watchdog* dog = watchdog_new(1);
        watchdog_add_puppy(dog, &unit);

        watchdog_check_puppies(dog);
        watchdog_delete(dog);
        assert(unit.status == WATCH_DOG_STATUS_FINISHED);
    }

}

static void lock_unlock_test() {
    WatchdogControlUnit unit = {.status = WATCH_DOG_STATUS_UP, .thread_id = 0, 
                            .unit_mutex = PTHREAD_MUTEX_INITIALIZER};

    watchdog_unit_lock(&unit);
    watchdog_unit_unlock(&unit);
}

static void clear_test() {
    WatchdogControlUnit unit = {.status = WATCH_DOG_STATUS_ROUGE, .thread_id = 0, 
                            .unit_mutex = PTHREAD_MUTEX_INITIALIZER};
    Watchdog* dog = watchdog_new(1);

    watchdog_add_puppy(dog, &unit);
    watchdog_clear(dog);
    
    assert(watchdog_add_puppy(dog, &unit));

    watchdog_delete(dog);
}

static void create_destroy_test() {

    Watchdog* dog = watchdog_new(1);
    watchdog_delete(dog);

    dog = watchdog_new(0);
    assert(dog == NULL);
}

static void add_remove_test() {

    WatchdogControlUnit unit = {.status = WATCH_DOG_STATUS_FINISHED, .thread_id = 0, 
                        .unit_mutex = PTHREAD_MUTEX_INITIALIZER};
    WatchdogControlUnit unit2 = {.status = WATCH_DOG_STATUS_FINISHED, .thread_id = 0, 
                        .unit_mutex = PTHREAD_MUTEX_INITIALIZER};
    Watchdog* dog = watchdog_new(1);

    assert(!watchdog_add_puppy(dog, NULL));

    assert(watchdog_add_puppy(dog, &unit));

    assert(!watchdog_add_puppy(dog, &unit2));

    watchdog_remove_puppy(dog, &unit);
    watchdog_remove_puppy(dog, NULL); /*This shall not cause crash*/
    assert(watchdog_add_puppy(dog, &unit2));
    watchdog_delete(dog);

}
    

int main() {

    create_destroy_test();
    add_remove_test();
    mark_and_sweep_test();
    clear_test();
    ping_finish_test();
    lock_unlock_test();
    number_of_units_test();
    create_unit_destroy_test();

    return 0;
}

#include <pthread.h>
#include <assert.h>
#include "watchdog.h"


static void create_destroy_test();
static void add_remove_test();
static void mark_and_sweep_test();
static void clear_test();
static void ping_finish_test();
static void* for_termination_thread(void* args);
static void lock_unlock_test();
static void number_of_units_test();

static pthread_mutex_t test_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t test_cond = PTHREAD_COND_INITIALIZER;

void ping_finish_test() {
    Watchdog_Control_Unit unit = {.status = WATCH_DOG_STATUS_DOWN, .thread_id = 0, .unit_mutex = PTHREAD_MUTEX_INITIALIZER};

    watchdog_ping(&unit);

    assert(unit.status == WATCH_DOG_STATUS_UP);

    watchdog_finish(&unit);

    assert(unit.status == WATCH_DOG_STATUS_FINISHED);
}

static void* for_termination_thread(void* args) {
    
    while(true) {
        pthread_cond_wait(&test_cond, &test_mutex);
    };

    return NULL;
}

static void number_of_units_test() {
    Watchdog* dog = watchdog_new(1);
    Watchdog_Control_Unit unit = {.status = WATCH_DOG_STATUS_UP, .thread_id = 0, 
                            .unit_mutex = PTHREAD_MUTEX_INITIALIZER};

    assert(watchdog_number_of_units(dog) == 0);
    watchdog_add_puppy(dog, &unit);
    assert(watchdog_number_of_units(dog) == 1);
    watchdog_remove_puppy(dog, &unit);
    assert(watchdog_number_of_units(dog) == 0);
}

static void mark_and_sweep_test() {
    
    {
        Watchdog_Control_Unit unit = {.status = WATCH_DOG_STATUS_UP, .thread_id = 0, 
                            .unit_mutex = PTHREAD_MUTEX_INITIALIZER};
        Watchdog* dog = watchdog_new(1);
        watchdog_add_puppy(dog, &unit);

        watchdog_mark_and_sweep(dog);
        
        assert(unit.status == WATCH_DOG_STATUS_DOWN);
        
        watchdog_delete(dog);
    }

    {
        Watchdog_Control_Unit unit = {.status = WATCH_DOG_STATUS_FINISHED, .thread_id = 0, 
                            .unit_mutex = PTHREAD_MUTEX_INITIALIZER};
        Watchdog* dog = watchdog_new(1);
        watchdog_add_puppy(dog, &unit);

        watchdog_mark_and_sweep(dog);
        watchdog_delete(dog);
        assert(unit.status == WATCH_DOG_STATUS_FINISHED);
    }

    {
        Watchdog_Control_Unit unit = {.status = WATCH_DOG_STATUS_DOWN, .thread_id = 0, 
                            .unit_mutex = PTHREAD_MUTEX_INITIALIZER};
        Watchdog* dog = watchdog_new(1);
        watchdog_add_puppy(dog, &unit);

        void* res = NULL;

        pthread_t thread1;
        pthread_create(&thread1, NULL, for_termination_thread, NULL);
        unit.thread_id = thread1;

        watchdog_mark_and_sweep(dog);
        pthread_join(thread1, &res);
        assert(unit.status == WATCH_DOG_STATUS_CANCELED);
        watchdog_delete(dog);
        assert(res == PTHREAD_CANCELED);
    }

}

static void lock_unlock_test() {
    Watchdog_Control_Unit unit = {.status = WATCH_DOG_STATUS_UP, .thread_id = 0, 
                            .unit_mutex = PTHREAD_MUTEX_INITIALIZER};

    watchdog_unit_lock(&unit);
    watchdog_unit_unlock(&unit);
}

static void clear_test() {
    Watchdog_Control_Unit unit = {.status = WATCH_DOG_STATUS_CANCELED, .thread_id = 0, 
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

    Watchdog_Control_Unit unit = {.status = WATCH_DOG_STATUS_FINISHED, .thread_id = 0, 
                        .unit_mutex = PTHREAD_MUTEX_INITIALIZER};
    Watchdog_Control_Unit unit2 = {.status = WATCH_DOG_STATUS_FINISHED, .thread_id = 0, 
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

    return 0;
}
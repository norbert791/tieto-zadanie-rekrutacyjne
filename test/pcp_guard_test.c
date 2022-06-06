#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include "pcp_guard.h"

typedef struct test_args {
    PCP_Guard* guard;
    bool* var;
}test_args;

/**
 * @brief This test succeds if no error occurs
 * 
 */
static void lock();
static void notify_producer();
static void notify_consumer();
static void wait_for_producer();
static void wait_for_consumer();
static void timed_wait();

static void* consumer_wait(void* args);
static void* producer_wait(void* args);

static void* producer_wait(void* args) {
     
    volatile bool* temp = ((test_args*)args)->var;
    PCP_Guard* guard = ((test_args*)args)->guard;
    pcp_guard_lock(guard);
    *temp = false;
    pcp_guard_notify_consumer(guard);
    pcp_guard_wait_for_consumer(guard);
    pcp_guard_unlock(guard);

    return NULL;
}

static void* consumer_wait(void* args) {
    volatile bool* temp = ((test_args*)args)->var;
    PCP_Guard* guard = ((test_args*)args)->guard;
    pcp_guard_lock(guard);
    *temp = false;
    pcp_guard_notify_producer(guard);
    pcp_guard_wait_for_producer(guard);
    pcp_guard_unlock(guard);

    return NULL;

}

static void lock() {

    PCP_Guard guard;
    if (pcp_guard_init(&guard) != PCP_SUCCESS) {
        perror("Warning: guard init failed during test\n");
        return;
    }
    pcp_guard_lock(&guard);
    pcp_guard_unlock(&guard);
    pcp_guard_destroy(&guard);
}

static void notify_producer() {
    PCP_Guard guard;
    if (pcp_guard_init(&guard) != PCP_SUCCESS) {
        perror("Warning: guard init failed during test\n");
        return;
    }
    pcp_guard_notify_producer(&guard);
    pcp_guard_destroy(&guard);
}

static void notify_consumer() {
    PCP_Guard guard;
    if (pcp_guard_init(&guard) != PCP_SUCCESS) {
        perror("Warning: guard init failed during test\n");
        return;
    }
    pcp_guard_notify_consumer(&guard);
    pcp_guard_destroy(&guard);
}

static void wait_for_consumer() {

    pthread_t consumer;
    PCP_Guard guard;
    pcp_guard_init(&guard);

    bool var = true;

    test_args args = {.guard = &guard, .var = &var};

    if (pthread_create(&consumer, NULL, producer_wait, &args) == -1) {
        perror("Warning: thread creation failed\n");
        return ;
    }

    while(true) {
        pcp_guard_lock(&guard);
        if (var) {
            pcp_guard_wait_for_producer(&guard);
            pcp_guard_notify_producer(&guard);
            pcp_guard_unlock(&guard);
            break;
        }
        else {
            pcp_guard_notify_producer(&guard);
            pcp_guard_unlock(&guard);
            break;
        }
    }
    
    pthread_join(consumer, NULL);
    assert(!var);
    pcp_guard_destroy(&guard);
}

static void wait_for_producer() {

    pthread_t consumer;
    PCP_Guard guard;
    pcp_guard_init(&guard);

    bool var = true;

    test_args args = {.guard = &guard, .var = &var};

    if (pthread_create(&consumer, NULL, consumer_wait, &args) == -1) {
        perror("Warning: thread creation failed\n");
        return;
    }

    while(true) {
        pcp_guard_lock(&guard);
        if (var) {
            pcp_guard_wait_for_consumer(&guard);
            pcp_guard_notify_consumer(&guard);
            pcp_guard_unlock(&guard);
            break;
        }
        else {
            pcp_guard_notify_consumer(&guard);
            pcp_guard_unlock(&guard);
            break;
        }
    }

    pthread_join(consumer, NULL);
    assert(!var);
    pcp_guard_destroy(&guard);
}

static void timed_wait() {
    PCP_Guard guard = PCP_GUARD_INITIALIZER;
    const struct timespec time_temp = {.tv_nsec = 100, .tv_sec = 0};

    /*Fails if test hangs up*/
    pcp_guard_timed_wait_for_producer(&guard, &time_temp);
}

int main() {
    lock();
    notify_producer();
    notify_consumer();
    wait_for_producer();
    wait_for_consumer();
    timed_wait();
}
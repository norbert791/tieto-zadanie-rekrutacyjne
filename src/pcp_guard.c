#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include "pcp_guard.h" 

EPCPStatus pcp_guard_init(PCPGuard* const guard) {

    if (guard == NULL) {
        return NULL_ARGUMENT;
    }   
    if (pthread_mutex_init(&(guard->mutex), NULL) != 0) {
        return MUTEX_FAILURE;
    }
    if (pthread_cond_init(&(guard->consumer), NULL) != 0) {
        pthread_mutex_destroy(&(guard->mutex));
        return PRODUCER_FAILURE;
    }
    if (pthread_cond_init(&(guard->producer), NULL) != 0) {
        pthread_mutex_destroy(&(guard->mutex));
        pthread_cond_destroy(&(guard->consumer));
        return CONSUMER_FAILURE;
    }

    return PCP_SUCCESS;

}

EPCPStatus pcp_guard_destroy(PCPGuard* const guard) {

    if (pthread_mutex_destroy(&(guard->mutex)) != 0) {
        return MUTEX_FAILURE;
    }
    if (pthread_cond_destroy(&(guard->consumer)) != 0) {
        return PRODUCER_FAILURE;
    }
    if (pthread_cond_destroy(&(guard->producer)) !=0) {
        return CONSUMER_FAILURE;
    }
    return PCP_SUCCESS;
}

int pcp_guard_lock(PCPGuard* guard);

int pcp_guard_unlock(PCPGuard* guard);

int pcp_guard_wait_for_producer(PCPGuard* guard);

int pcp_guard_wait_for_consumer(PCPGuard* guard);

int pcp_guard_notify_producer(PCPGuard* guard);

int pcp_guard_notify_consumer(PCPGuard* guard);

int pcp_guard_timed_wait_for_producer(PCPGuard* restrict guard, const struct timespec *restrict abstime);
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include "pcp_guard.h"

PCP_STATUS pcp_guard_init(PCP_Guard* guard) {

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

PCP_STATUS pcp_guard_destroy(PCP_Guard* guard) {

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

int pcp_guard_lock(PCP_Guard* guard) {
    return pthread_mutex_lock(&(guard->mutex));
}

int pcp_guard_unlock(PCP_Guard* guard) {
    return pthread_mutex_unlock(&(guard->mutex));
}

int pcp_guard_wait_for_producer(PCP_Guard* guard) {
    return pthread_cond_wait(&(guard->consumer), &(guard->mutex));
}

int pcp_guard_wait_for_consumer(PCP_Guard* guard) {
    return pthread_cond_wait(&(guard->producer), &(guard->mutex));
}

int pcp_guard_notify_producer(PCP_Guard* guard) {
    return pthread_cond_signal(&(guard->producer));
}

int pcp_guard_notify_consumer(PCP_Guard* guard) {
    return pthread_cond_signal(&(guard->consumer));
}
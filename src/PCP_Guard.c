#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include "pcp_guard.h"

PCP_STATUS PCP_Guard_init(PCP_Guard* guard) {

    if (guard == NULL) {
        return NULL_ARGUMENT;
    }   
    if (pthread_mutex_init(&(guard->mutex), NULL) == -1) {
        return MUTEX_FAILURE;
    }
    if (pthread_cond_init(&(guard->consumer), NULL) == -1) {
        pthread_mutex_destroy(&(guard->mutex));
        return PRODUCER_FAILURE;
    }
    if (pthread_cond_init(&(guard->producer), NULL) == -1) {
        pthread_mutex_desrtoy(&(guard->mutex));
        pthread_cond_destroy(&(guard->consumer));
        return CONSUMER_FAILURE;
    }

    return PCP_SUCCESS;

}

PCP_STATUS PCP_Guard_destroy(PCP_Guard* guard) {

    if (pthread_mutex_destroy(&(guard->mutex)) ==  -1) {
        return MUTEX_FAILURE;
    }
    if (pthread_cond_destroy(&(guard->consumer)) == -1) {
        return PRODUCER_FAILURE;
    }
    if (pthread_mutexattr_destroy(&(guard->producer)) == -1) {
        return CONSUMER_FAILURE;
    }
    return PCP_SUCCESS;
}

int PCP_Guard_lock(PCP_Guard* guard) {
    return pthread_mutex_lock(&(guard->mutex)) == 0;
}

int PCP_Guard_unlock(PCP_Guard* guard) {
    return pthread_mutex_unlock(&(guard->mutex)) == 0;
}

int PCP_Guard_wait_for_producer(PCP_Guard* guard) {
    return pthread_cond_wait(&(guard->consumer), &(guard->mutex));
}

int PCP_Guard_wait_for_consumer(PCP_Guard* guard) {
    return pthread_cond_wait(&(guard->producer), &(guard->mutex));
}

int PCP_Guard_notify_producer(PCP_Guard* guard) {
    return pthread_cond_signal(&(guard->producer));
}

int PCP_Guard_notify_consumer(PCP_Guard* guard) {
    return pthread_cond_signal(&(guard->consumer));
}
#ifndef PCP_GUARD H
#define PCP_GUARD_H

#include <pthread.h>

typedef enum PCP_STATUS {
    PCP_SUCCESS,
    MUTEX_FAILURE,
    PRODUCER_FAILURE,
    CONSUMER_FAILURE,
    NULL_ARGUMENT,
}PCP_STATUS;

typedef struct PCP_Guard {
    pthread_mutex_t mutex;
    pthread_cond_t producer;
    pthread_cond_t consumer;
} PCP_Guard;


PCP_STATUS pcp_guard_init(PCP_Guard* guard);

PCP_STATUS pcp_guard_destroy(PCP_Guard* guard);

int pcp_guard_lock(PCP_Guard* guard);

int pcp_guard_unlock(PCP_Guard* guard);

int pcp_guard_notify_producer(PCP_Guard* guard);

int pcp_guard_notify_consumer(PCP_Guard* guard);

int pcp_guard_wait_for_producer(PCP_Guard* guard);

int pcp_guard_wait_for_consumer(PCP_Guard* guard);

#endif
/**
 * @file pcp_guard.h
 * @brief Structures and wrapper functions for solving PCP problem.
 * 
 */

#ifndef PCP_GUARD_H
#define PCP_GUARD_H

#include <pthread.h>
#include <time.h>

/**
 * @brief Constants returned by some of the functions
 * 
 */
typedef enum PCP_STATUS {
    PCP_SUCCESS,
    MUTEX_FAILURE,
    PRODUCER_FAILURE,
    CONSUMER_FAILURE,
    NULL_ARGUMENT,
}PCP_STATUS;

#define PCP_GUARD_INITIALIZER {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER}

/**
 * @brief Structure combining two conditional variables,
 * one for producer and one for consumer and mutex
 * 
 */
typedef struct PCP_Guard {
    pthread_mutex_t mutex;
    pthread_cond_t producer;
    pthread_cond_t consumer;
} PCP_Guard;


/**
 * @brief Initialize fields in target PCP_Guard.
 * If initialization of one of the fields fails, then all the other
 * fields are destroyed. Calling this function on PCP_Guard whose field(s) is(are)
 * initialized is undefined behaviour.
 * 
 * @param guard pointer to uninitialized PCP_Guard.
 * @return PCP_STATUS On success PCP_SUCCESS is returned. On failure,
 * returns constant indicating which part of the initialization failed.
 * Additionally errno may be raised. @see man pthread_mutex_init(3), @see man pthread_cond_init(3)
 */
PCP_STATUS pcp_guard_init(PCP_Guard* guard);

/**
 * @brief Destroy fields fields in target PCP_Guard. It shall be safe to call this function
 * on pointer to initialized guard unless said guard is in used or will be used before this function returns.
 * Otherwise, the behaviour is undefined, although PCP_STATUS indicating which part of the destruction failed may be returned
 * and additional errno flag may be raised @see pthread_mutex_destroy(3) @see pthread_cond_destroy(3); in such case
 * the state of the guard is undefined though.
 * 
 * @param guard pointer to initialized PCP_Guard
 * @return PCP_STATUS PCP PCP_SUCCESS on success.
 */
PCP_STATUS pcp_guard_destroy(PCP_Guard* guard);

/**
 * @brief wrapper for pthread_mutex_lock. The function behaves exactly as though
 * pthread_mutex_lock(PCP_Guard->mutex) would be called. @see man pthread_mutex_lock(3)
 * 
 * @param guard pointer to valid PCP_Guard
 * @return int return value of pthread_mutex_lock
 */
int pcp_guard_lock(PCP_Guard* guard);

/**
 * @brief wrapper for pthread_mutex_unlock. The function behaves exactly as though
 * pthread_mutex_lock(PCP_Guard->mutex) would be called. @see man pthread_mutex_unlock(3)
 * 
 * @param guard pointer to valid PCP_Guard
 * @return int return value of pthread_mutex_unlock
 */
int pcp_guard_unlock(PCP_Guard* guard);

/**
 * @brief wrapper for pthread_cond_signal. The function behaves exactly as though
 * pthread_cond_signal(PCP_Guard->producer) would be called. @see man pthread_cond_signal(3)
 * 
 * @param guard pointer to valid PCP_Guard
 * @return int return value of pthread_cond_signal
 */
int pcp_guard_notify_producer(PCP_Guard* guard);

/**
 * @brief wrapper for pthread_cond_signal. The function behaves exactly as though
 * pthread_cond_signal(PCP_Guard->consumer) would be called. @see man pthread_cond_signal(3)
 * 
 * @param guard pointer to valid PCP_Guard
 * @return int return value of pthread_cond_signal
 */
int pcp_guard_notify_consumer(PCP_Guard* guard);

/**
 * @brief wrapper for pthread_cond_wait. The function behaves exactly as though
 * pthread_cond_wait(PCP_Guard->consumer) would be called. @see man pthread_cond_wait(3)
 * 
 * @param guard pointer to valid PCP_Guard
 * @return int return value of pthread_cond_signal
 */
int pcp_guard_wait_for_producer(PCP_Guard* guard);

/**
 * @brief wrapper for pthread_cond_wait. The function behaves exactly as though
 * pthread_cond_wait(PCP_Guard->producer) would be called. @see man pthread_cond_wait(3)
 * 
 * @param guard pointer to valid PCP_Guard
 * @return int return value of pthread_cond_signal
 */
int pcp_guard_wait_for_consumer(PCP_Guard* guard);


/**
 * @brief wrappter for pthread_cond_timedwait.
 * The function behaves exactly as though pthrad_cond_timedwait(PCP_Guard->producer, (abstime)) would be called
 * @see man pthread_cond_timedwait(3)
 * 
 * @param guard pointer to valid PCP_Guard
 * @return int return value of pthread_cond_timedwait
 */
int pcp_guard_timed_wait_for_producer(PCP_Guard* guard, const struct timespec *restrict abstime);

#endif
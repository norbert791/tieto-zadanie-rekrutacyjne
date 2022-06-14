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
typedef enum EPCPStatus {
    PCP_SUCCESS,
    MUTEX_FAILURE,
    PRODUCER_FAILURE,
    CONSUMER_FAILURE,
    NULL_ARGUMENT,
}EPCPStatus;

#define PCP_GUARD_INITIALIZER {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER}

/**
 * @brief Structure combining two conditional variables,
 * one for producer and one for consumer and mutex
 * 
 */
typedef struct PCPGuard {
    pthread_mutex_t mutex;
    pthread_cond_t producer;
    pthread_cond_t consumer;
} PCPGuard;


/**
 * @brief Initialize fields in target PCPGuard.
 * If initialization of one of the fields fails, then all the other
 * fields are destroyed. Calling this function on PCPGuard whose field(s) is(are)
 * initialized is undefined behaviour.
 * 
 * @param guard pointer to uninitialized PCPGuard.
 * @return EPCPStatus On success PCP_SUCCESS is returned.
 * @return On failure returns constant indicating which part of the initialization failed.
 * Additionally errno may be set. @see man pthread_mutex_init(3), @see man pthread_cond_init(3)
 */
EPCPStatus pcp_guard_init(PCPGuard* guard);

/**
 * @brief Destroy fields fields in target PCPGuard. It shall be safe to call this function
 * on pointer to initialized guard unless said guard is in used or will be used before this function returns.
 * Otherwise, the behaviour is undefined, although EPCPStatus indicating which part of the destruction failed may be returned
 * and additionally errno may be set @see pthread_mutex_destroy(3) @see pthread_cond_destroy(3); in such case
 * the state of the guard is undefined though.
 * 
 * @param guard pointer to initialized PCPGuard
 * @return EPCPStatus PCP PCP_SUCCESS on success.
 */
EPCPStatus pcp_guard_destroy(PCPGuard* guard);

/**
 * @brief wrapper for pthread_mutex_lock. The function behaves exactly as though
 * pthread_mutex_lock(PCPGuard->mutex) would be called. @see man pthread_mutex_lock(3)
 * 
 * @param guard pointer to valid PCPGuard
 * @return int return value of pthread_mutex_lock
 */
inline int pcp_guard_lock(PCPGuard* guard) {
    return pthread_mutex_lock(&(guard->mutex));
}

/**
 * @brief wrapper for pthread_mutex_unlock. The function behaves exactly as though
 * pthread_mutex_lock(PCPGuard->mutex) would be called. @see man pthread_mutex_unlock(3)
 * 
 * @param guard pointer to valid PCPGuard
 * @return int return value of pthread_mutex_unlock
 */
inline int pcp_guard_unlock(PCPGuard* guard) {
    return pthread_mutex_unlock(&(guard->mutex));
}

/**
 * @brief wrapper for pthread_cond_signal. The function behaves exactly as though
 * pthread_cond_signal(PCPGuard->producer) would be called. @see man pthread_cond_signal(3)
 * 
 * @param guard pointer to valid PCPGuard
 * @return int return value of pthread_cond_signal
 */
inline int pcp_guard_notify_producer(PCPGuard* guard) {
    return pthread_cond_signal(&(guard->producer));
}

/**
 * @brief wrapper for pthread_cond_signal. The function behaves exactly as though
 * pthread_cond_signal(PCPGuard->consumer) would be called. @see man pthread_cond_signal(3)
 * 
 * @param guard pointer to valid PCPGuard
 * @return int return value of pthread_cond_signal
 */
inline int pcp_guard_notify_consumer(PCPGuard* guard) {
    return pthread_cond_signal(&(guard->consumer));
}

/**
 * @brief wrapper for pthread_cond_wait. The function behaves exactly as though
 * pthread_cond_wait(PCPGuard->consumer) would be called. @see man pthread_cond_wait(3)
 * 
 * @param guard pointer to valid PCPGuard
 * @return int return value of pthread_cond_signal
 */
inline int pcp_guard_wait_for_producer(PCPGuard* guard) {
    return pthread_cond_wait(&(guard->consumer), &(guard->mutex));
}

/**
 * @brief wrapper for pthread_cond_wait. The function behaves exactly as though
 * pthread_cond_wait(PCPGuard->producer) would be called. @see man pthread_cond_wait(3)
 * 
 * @param guard pointer to valid PCPGuard
 * @return int return value of pthread_cond_signal
 */
inline int pcp_guard_wait_for_consumer(PCPGuard* guard) {
    return pthread_cond_wait(&(guard->producer), &(guard->mutex));
}


/**
 * @brief wrapper for pthread_cond_timedwait.
 * The function behaves exactly as though pthrad_cond_timedwait(PCPGuard->producer, (abstime)) would be called
 * @see man pthread_cond_timedwait(3)
 * 
 * @param guard pointer to valid PCPGuard
 * @return int return value of pthread_cond_timedwait
 */
inline int pcp_guard_timed_wait_for_producer(PCPGuard* restrict guard, const struct timespec *restrict abstime) {
    return pthread_cond_timedwait(&(guard->producer), &(guard->mutex), abstime);
}

#endif

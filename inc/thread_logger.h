/**
 * @file thread_logger.h
 * @brief thread_logger is function working in loop until *is_working is true
 *  The function reads pointers to logger_payloads from CircularBuffer and creates
 *  log entry in logger_output file. After payload has been successfully stored, the payload is
 *  deleted. Hence it is not safe to refer to payloads in any other way once they've been inserted
 *  into the buffer.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdbool.h>
#include "pcp_guard.h"
#include "circular_buffer.h"
#include "watchdog.h"
#include "logger_payload.h"

typedef struct ThreadLoggerArguments {

    PCPGuard* buffer_guard;
    CircularBuffer* logger_payload_pointer_buffer;
    bool* is_working;
    pthread_mutex_t* is_working_mutex;
    FILE* logger_output;
    WatchdogControlUnit* control_unit;

} ThreadLoggerArguments;

void* thread_logger(void* thread_logger_args);

/**
 * @brief wrapper function for sending payload to logger
 * 
 * @param payload_buffer_guard pointer to valid pcp_guard protecting payload buffer
 * @param payload_ptr_buffer pointer to valid pointer buffer
 * @param message message that will be inserted into payload
 * @param type type of the message
 * @return true iff function sent payload successfully
 */
inline bool thread_logger_send_log(PCPGuard* restrict payload_buffer_guard, 
                                            CircularBuffer* restrict payload_ptr_buffer, 
                                            const char message[restrict static 1], ELoggerPayloadType type) {

    /*Note: it will be thread_logger's responsibility to delete payload */
    LoggerPayload* payload = logger_payload_new(type, message);
    if (payload == NULL) {
        return false;
    }
    pcp_guard_lock(payload_buffer_guard);
    while (circular_buffer_write_available(payload_ptr_buffer) == 0) {
        pcp_guard_wait_for_consumer(payload_buffer_guard);
    }
    circular_buffer_insert_single(payload_ptr_buffer, &payload);
    pcp_guard_notify_producer(payload_buffer_guard);
    pcp_guard_unlock(payload_buffer_guard);
    
    return true;
}

#endif

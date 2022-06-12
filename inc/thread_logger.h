#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdbool.h>
#include "pcp_guard.h"
#include "circular_buffer.h"
#include "watchdog.h"
#include "logger_payload.h"

typedef struct thread_logger_arguments {

    PCP_Guard* buffer_guard;
    Circular_Buffer* logger_payload_pointer_buffer;
    bool* is_working;
    pthread_mutex_t* is_working_mutex;
    FILE* logger_output;
    Watchdog_Control_Unit* control_unit;

} thread_logger_arguments;

void* thread_logger(void* thread_logger_args);

inline bool thread_logger_send_log(PCP_Guard* restrict payload_buffer_guard, 
                                            Circular_Buffer* restrict payload_ptr_buffer, 
                                            const char message[restrict static 1], logger_payload_type type) {

    /*Note: it will be thread_logger responsibility to delete payload */
    Logger_Payload* payload = logger_payload_new(type, message);
    if (payload == NULL) {
        return false;
    }
    pcp_guard_lock(payload_buffer_guard);
    if (circular_buffer_insert_single(payload_ptr_buffer, &payload) == 0) {
        pcp_guard_wait_for_consumer(payload_buffer_guard);
        circular_buffer_insert_single(payload_ptr_buffer, &payload);
        pcp_guard_notify_producer(payload_buffer_guard);
    }
    pcp_guard_unlock(payload_buffer_guard);
    
    return true;
}

#endif
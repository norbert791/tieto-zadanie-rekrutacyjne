#include <stdbool.h>
#include <time.h>
#include "thread_logger.h"
#include "logger_payload.h"


bool thread_logger_send_log(PCPGuard* restrict payload_buffer_guard, 
                                            CircularBuffer* restrict payload_ptr_buffer, 
                                            const char message[restrict static 1], ELoggerPayloadType type);

static inline void persist_to_file(FILE* logger_file, LoggerPayload* payload);

void* thread_logger(void* args) {

    if (args == NULL) {
        perror("logger: args equal to NULL\n");
        return NULL;
    }

    CircularBuffer* payload_buffer = NULL;
    PCPGuard* buffer_guard = NULL;
    pthread_mutex_t* working_mutex = NULL;
    bool* working = NULL;
    FILE* logger_file = NULL;
    WatchdogControlUnit* control_unit = NULL;
    const struct timespec cond_wait_time = {.tv_nsec = 0, .tv_sec = 1};

    {
        ThreadLoggerArguments* temp = args;

        payload_buffer = temp->logger_payload_pointer_buffer;
        buffer_guard = temp->buffer_guard;
        working_mutex = temp->is_working_mutex;
        working = temp->is_working;
        logger_file = temp->logger_output;
        control_unit = temp->control_unit;
    }

    if (payload_buffer == NULL || buffer_guard == NULL || working_mutex == NULL || working == NULL || logger_file == NULL || control_unit == NULL) {
        perror ("Logger: one of args argument equal to NULL\n");
        return NULL;
    }

    while (true) {
        watchdog_unit_atomic_ping(control_unit);
        pthread_mutex_lock(working_mutex);
        if (!*working) {
            pthread_mutex_unlock(working_mutex);
            watchdog_unit_atomic_finish(control_unit);
            break;
        }
        pthread_mutex_unlock(working_mutex);
        LoggerPayload* payload = NULL;
        pcp_guard_lock(buffer_guard);
        if (circular_buffer_remove_single(payload_buffer, &payload) == 0) {
            pcp_guard_timed_wait_for_producer(buffer_guard, &cond_wait_time);
            pcp_guard_notify_producer(buffer_guard);   
            pcp_guard_unlock(buffer_guard);
            continue;         
        }
        pcp_guard_unlock(buffer_guard);
        persist_to_file(logger_file, payload);
        
        logger_payload_delete(payload);
        payload = NULL;
    }

    return NULL;
}

static inline void persist_to_file(FILE* restrict logger_file, LoggerPayload* restrict payload) {

    char time_buffer[26];
    time_t time_now;
    time(&time_now);
    if (ctime_r(&time_now, time_buffer) == NULL) {
        fprintf(logger_file, "LOGGER ERROR: TIME\n");
        fflush(logger_file);
        return;
    }
    fprintf(logger_file, "Event: %s\nTime: %sMessage: %s \n ----------\n", 
            logger_payload_type_to_str(logger_payload_get_type(payload)), time_buffer, logger_payload_get_message(payload));
    fflush(logger_file);
}

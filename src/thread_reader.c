#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include "thread_reader.h"
#include "circular_buffer.h"
#include "pcp_guard.h"
#include "thread_logger.h"


/*
 * Clean up before leaving. Let us consider the following interleaving:
 * 
 * buffer has 0 bytes for read, is_working = true
 * 
 * parsing thread -> checks is_working and continues the job
 * Program finishes -> is_working is set to false
 * Writer -> checks is_working and leaves
 * parsing thread -> waits for bytes to read
 */
static inline void finalize(CircularBuffer* char_buffer, PCPGuard* char_buffer_guard);

void* thread_reader(void* reader_arguments) {
    /*Sanity check*/
    if (reader_arguments == NULL) {
        perror("NULL argument passed");
        return NULL;
    }

    const struct timespec sleep_time = {.tv_nsec = 0, .tv_sec = 1};
    PCPGuard* char_buffer_guard = NULL;
    PCPGuard* logger_buffer_guard = NULL;
    CircularBuffer* logger_buffer = NULL;
    CircularBuffer* char_buffer = NULL;
    WatchdogControlUnit* control_unit = NULL;
    bool* is_working = NULL;
    pthread_mutex_t* working_mtx = NULL;
    FILE* input_file = NULL;

    {
        ThreadReaderArguments* temp = reader_arguments;

        char_buffer_guard = temp->char_buffer_guard;
        char_buffer = temp->char_buffer;
        logger_buffer = temp->logger_buffer;
        logger_buffer_guard = temp->logger_buffer_guard;
        control_unit = temp->control_unit;
        is_working = temp->working;
        working_mtx = temp->working_mutex;
        input_file = temp->input_file;

        temp = NULL;
    }

    /*Sanity check*/
    if (char_buffer == NULL || char_buffer_guard == NULL || logger_buffer == NULL || logger_buffer_guard == NULL
        || is_working == NULL || working_mtx == NULL || input_file == NULL || control_unit == NULL) {
        perror("One of arguments was NULL");
        return NULL;
    }

    while (true) {
        pthread_mutex_lock(working_mtx);
        if (!*is_working) {
            finalize(char_buffer, char_buffer_guard);
            pthread_mutex_unlock(working_mtx);
            watchdog_unit_atomic_finish(control_unit);
            break;
        }
        pthread_mutex_unlock(working_mtx);
        
        int input_char_int = fgetc(input_file);
        
        if (input_char_int != EOF) {
            char input_char = (char) input_char_int;
            pcp_guard_lock(char_buffer_guard);

            if (circular_buffer_insert_single(char_buffer, &input_char) == 0) {
                pcp_guard_wait_for_consumer(char_buffer_guard);
                circular_buffer_insert_single(char_buffer, &input_char);
            }
            pcp_guard_notify_consumer(char_buffer_guard);
            pcp_guard_unlock(char_buffer_guard);
        }
        else if (feof(input_file)) {
            watchdog_unit_atomic_ping(control_unit);
            
            if (nanosleep(&sleep_time, NULL) != 0) {
                errno = 0;
                thread_logger_send_log(logger_buffer_guard, logger_buffer,
                                       "Sleep error\n", LOGGER_PAYLOAD_TYPE_ERROR);
            }
            fflush(input_file);
            rewind(input_file);
        }
        else if (ferror(input_file)) {
            clearerr(input_file);
            thread_logger_send_log(logger_buffer_guard, logger_buffer,
                                   "File error during read attempt\n", LOGGER_PAYLOAD_TYPE_ERROR);
        }
    }
    return NULL;
}

static inline void finalize(CircularBuffer* char_buffer, PCPGuard* char_buffer_guard) {
    /*lock on buffer guard*/
    pcp_guard_lock(char_buffer_guard);
    /*Insert some garbage that will be discarded anyway, 
    NOTE: if buffer is full, nothing will happen*/
    const char temp = ' ';
    circular_buffer_insert_single(char_buffer, &temp);
    /*If reader is lock on buffer guard, then it will be released after unlock*/
    pcp_guard_notify_consumer(char_buffer_guard);
    /*Release buffer guard*/
    pcp_guard_unlock(char_buffer_guard);
}

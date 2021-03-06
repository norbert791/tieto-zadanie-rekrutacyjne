#include <string.h>
#include "thread_parser.h"
#include "proc_parser.h"
#include "pcp_guard.h"
#include "thread_logger.h"


/**
 * @brief Clean up before leaving. 
 * Let us consider the following interleaving:
 * 
 * buffer has 0 bytes to read, is_working = true
 * 
 * Reader -> checks is_working and continues the job
 * Program finishes -> is_working is set to false
 * Writer -> checks is_working and leaves
 * Reader -> waits for bytes to read
 */
static inline void finalize_write(CircularBuffer* char_buffer, PCPGuard* guard);

/**
 * @brief Clean up before leaving 
 * Situation similar to @see finalize_write
 */
static inline void finalize_read(CircularBuffer* char_buffer, PCPGuard* guard);

void* thread_parser(void* args) {
    if (args == NULL) {
        perror("Parser: null argument was given\n");
        return NULL;
    }

    enum {
        temporary_buffer_size = 800,
        previous_usage_size = 100,
    };

    CircularBuffer* char_buffer = NULL;
    CircularBuffer* double_buffer = NULL;
    CircularBuffer* logger_buffer = NULL;
    PCPGuard* logger_guard = NULL;
    PCPGuard* char_buffer_guard = NULL;
    PCPGuard* double_buffer_guard = NULL;
    WatchdogControlUnit* control_unit = NULL;
    bool* is_working = NULL;
    pthread_mutex_t* working_mtx = NULL;

    size_t index = 0;
    size_t computed_core = 0;
    char temporary_buffer[temporary_buffer_size];
    uint64_t parsed_data[10] = {0};
    ProcParserCpuTime previous_usage[previous_usage_size] = {0};

    {
        ThreadParserArguments* temp = args;

        char_buffer = temp->char_buffer;
        double_buffer = temp->double_buffer;
        logger_buffer = temp->logger_buffer;
        char_buffer_guard = temp->char_buffer_guard;
        double_buffer_guard = temp->double_buffer_guard;
        logger_guard = temp->logger_buffer_guard;
        is_working = temp->is_working;
        working_mtx = temp->working_mutex;
        control_unit = temp->control_unit;
    }

    /*sanity check*/
    if (char_buffer == NULL || double_buffer == NULL || logger_buffer == NULL || char_buffer_guard == NULL 
        || double_buffer_guard == NULL || logger_guard == NULL || is_working == NULL || working_mtx == NULL 
        || control_unit == NULL) {
        
        perror("Parser: One of arguments equal to NULL\n");
        return NULL;
    }

    while (true) {
        pthread_mutex_lock(working_mtx);
        if (!*is_working) {
            pthread_mutex_unlock(working_mtx);
            finalize_read(char_buffer, char_buffer_guard);
            finalize_write(double_buffer, double_buffer_guard);
            watchdog_unit_atomic_finish(control_unit);
            break;
        }
        pthread_mutex_unlock(working_mtx);
        pcp_guard_lock(char_buffer_guard);

        char input_char;
        if (circular_buffer_remove_single(char_buffer, &input_char) == 0) {
            pcp_guard_wait_for_producer(char_buffer_guard);
            circular_buffer_remove_single(char_buffer, &input_char);   
        }
        pcp_guard_notify_producer(char_buffer_guard);
        pcp_guard_unlock(char_buffer_guard);

        if (input_char == '\n') {
            
            temporary_buffer[index] = '\0';
            index = 0;
            int res = proc_parser_parse_line(temporary_buffer, parsed_data);

            if (res == PROC_PARSER_TOTAL_USAGE_LINE) {
                continue;
            }
            if (res == PROC_PARSER_SUCCESS) {
                ProcParserCpuTime current_usage = proc_parser_compute_core_time(parsed_data);
                double usage = proc_parser_cpu_time_compute_usage(
                                &previous_usage[computed_core], &current_usage) * 100;
                previous_usage[computed_core] = current_usage;
                computed_core++;

                pcp_guard_lock(double_buffer_guard);
                int c_b_result = circular_buffer_insert_single(double_buffer, &usage);
                if (c_b_result == 0) {
                    pcp_guard_wait_for_consumer(double_buffer_guard);
                    circular_buffer_insert_single(double_buffer, &usage);
                }
                pcp_guard_notify_consumer(double_buffer_guard);
                pcp_guard_unlock(double_buffer_guard);
            }
            else if (res == PROC_PARSER_DISCARD_LINE) {
                /*If compute_core == 0, then we are still receiving lines with data unrelated to threads*/
                if (computed_core == 0) {
                    continue;
                }
                computed_core = 0;
                double usage = THREAD_PARSER_END;
                pcp_guard_lock(double_buffer_guard);
                int c_b_result = circular_buffer_insert_single(double_buffer, &usage);

                if (c_b_result == 0) {
                    pcp_guard_wait_for_consumer(double_buffer_guard);
                    circular_buffer_insert_single(double_buffer, &usage);
                    pcp_guard_notify_consumer(double_buffer_guard);
                    
                }
                pcp_guard_unlock(double_buffer_guard);
            }
            else {
                thread_logger_send_log(logger_guard, logger_buffer,
                "Parser: Buffer is too small to accumulate parsed results\n", LOGGER_PAYLOAD_TYPE_WARNING);
            }
        }
        else {
            temporary_buffer[index] = input_char;
            index++;
            if (index == temporary_buffer_size) {
                if (strncmp(temporary_buffer, "cpu", 3) == 0) {
                    thread_logger_send_log(logger_guard, logger_buffer,
                    "Parser: Buffer size is too small to accumulate data sent by reader\n", LOGGER_PAYLOAD_TYPE_WARNING);
                }
                temporary_buffer[temporary_buffer_size - 1] = '\0';
                temporary_buffer[0] = input_char;
                index = 0;
            }
        }
        watchdog_unit_atomic_ping(control_unit);
    }
    return NULL;
}


static inline void finalize_read(CircularBuffer* char_buffer, PCPGuard* guard) {
    /*lock on buffer */
    pcp_guard_lock(guard);
    /*Insert some garbage that will be discarded anyway, 
    NOTE: if buffer is full, nothing will happen*/
    char temp;
    circular_buffer_remove_single(char_buffer, &temp);
    /*Notify reader. It will lock either lock on is_working or on buffer */
    pcp_guard_notify_producer(guard);
    /*Release buffer */
    pcp_guard_unlock(guard);
}


static inline void finalize_write(CircularBuffer* char_buffer, PCPGuard* guard) {
    /*lock on buffer */
    pcp_guard_lock(guard);
    /*Insert some garbage that will be discarded anyway, 
    NOTE: if buffer is full, nothing will happen*/
    double d = 1.0;
    circular_buffer_insert_single(char_buffer, &d);
    /*Notify reader. It will lock either lock on is_working or on buffer */
    pcp_guard_notify_consumer(guard);
    /*Release buffer */
    pcp_guard_unlock(guard);
}

#include <string.h>
#include "thread_parser.h"
#include "proc_parser.h"
#include "pcp_guard.h"

static inline void finilize_read(Circular_Buffer* char_buffer, PCP_Guard* guard);
static inline void finilize_write(Circular_Buffer* char_buffer, PCP_Guard* guard);

void* thread_parser(void* args) {

    /*Sanity check*/
    if (args == NULL) {
        /*Error handling*/
        return NULL;
    }

    Circular_Buffer* char_buffer;
    Circular_Buffer* double_buffer;
    PCP_Guard* char_buffer_guard;
    PCP_Guard* double_buffer_guard;
    volatile bool* is_working;
    pthread_mutex_t* working_mtx;

    size_t index = 0;
    /*TODO: find if there is upper limit for line width in proc/stat*/
    const size_t max_size = 400;
    char temporary_buffer[max_size];
    uint64_t currentData[10] = {0};
    uint64_t previousData[10] = {0};

    {
        thread_parser_arguments* temp = args;

        char_buffer = temp->char_buffer;
        double_buffer = temp->double_buffer;
        char_buffer_guard = temp->char_buffer_guard;
        double_buffer_guard = temp->double_buffer_guard;
        is_working = temp->is_working;
        working_mtx = temp->working_mutex;
    }

    /*sanity check*/
    if (char_buffer == NULL || double_buffer == NULL || is_working == NULL || working_mtx == NULL) {
        /*Error handling*/
        return NULL;
    }

    while (true) {
        pthread_mutex_lock(working_mtx);
        if (!*is_working) {
            finilize_read(char_buffer, char_buffer_guard);
            finilize_write(double_buffer, double_buffer_guard);
            pthread_mutex_unlock(working_mtx);
            break;
        }
        pthread_mutex_unlock(working_mtx);
        if (pcp_guard_lock(char_buffer_guard) != 0) {
            perror("Lock error: parser\n");
            continue;
        }

        char input_char;
        if (circular_buffer_remove_single(char_buffer, &input_char) == 0) {
            pcp_guard_wait_for_producer(char_buffer_guard);
            circular_buffer_remove_single(char_buffer, &input_char);   
        }
        pcp_guard_notify_producer(char_buffer_guard);
        pcp_guard_unlock(char_buffer_guard);

        if (input_char == '\n') {
            temporary_buffer[index] = '\0';
           // puts(temporary_buffer);
            index = 0;
            memcpy(previousData, currentData, 10);
            int res = proc_parser_parse_line(temporary_buffer, currentData);
            
            if (res == 10) {
                double temp = proc_parser_compute_core_usage(previousData, currentData);
                pcp_guard_lock(double_buffer_guard);
                int c_b_result = circular_buffer_insert_single(double_buffer, &temp);
                if (c_b_result == 0) {
                    pcp_guard_wait_for_consumer(double_buffer_guard);
                    circular_buffer_insert_single(double_buffer, &temp);
                }
                pcp_guard_notify_consumer(double_buffer_guard);
                pcp_guard_unlock(double_buffer_guard);
            }
            
            if (res == -2) {
                double temp = -1;
                pcp_guard_lock(double_buffer_guard);
                int c_b_result = circular_buffer_insert_single(double_buffer, &temp);

                if (c_b_result == 0) {
                    pcp_guard_wait_for_consumer(double_buffer_guard);
                    circular_buffer_insert_single(double_buffer, &temp);
                    pcp_guard_notify_consumer(double_buffer_guard);
                    
                }
                pcp_guard_unlock(double_buffer_guard);
            }
        }
        else {
            temporary_buffer[index] = input_char;
            index++;
            if (index == max_size) {
                /*Line width is larger than buffer - handle it accordingly */
                temporary_buffer[max_size - 1] = '\0';
                index = 0;
            }
        }
    }

    return NULL;
}


static inline void finilize_read(Circular_Buffer* char_buffer, PCP_Guard* guard) {
    /*lock on buffer */
    pcp_guard_lock(guard);
    /*Insert some garbage that will be discarded anyway, 
    NOTE: if buffer is full, nothing will happen*/
    char temp;
    circular_buffer_remove_single(char_buffer, &temp);
    /*Notify reader. It will lock either lock on is_working or on buffer */
    pcp_guard_notify_producer(guard);
    /*Relase buffer */
    pcp_guard_unlock(guard);
}

/*
 * Clean up before leaving. Let us consider the following interleaving:
 * 
 * buffer has 0 bytes for read, is_working = true
 * 
 * Reader -> checks is_working and continues the job
 * Program finishes -> is_working is set to false
 * Writer -> checks is_working and leaves
 * Reader -> waits for bytes to read
 */
static inline void finilize_write(Circular_Buffer* buffer, PCP_Guard* guard) {
    /*lock on buffer */
    pcp_guard_lock(guard);
    /*Insert some garbage that will be discarded anyway, 
    NOTE: if buffer is full, nothing will happen*/
    double d = 1.0;
    circular_buffer_insert_single(buffer, &d);
    /*Notify reader. It will lock either lock on is_working or on buffer */
    pcp_guard_notify_consumer(guard);
    /*Relase buffer */
    pcp_guard_unlock(guard);
}

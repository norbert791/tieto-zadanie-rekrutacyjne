#include <pthread.h>
#include <unistd.h>
#include "thread_printer.h"
#include "thread_parser.h"

/**
 * @brief Clean up before leaving:
 * Let us consider the following interleaving
 * 
 * buffer has 0 bytes for write, is_working = true
 * 
 * writer -> checks is_working and continues the job
 * Program finishes -> is_working is set to false
 * Writer -> checks is_working and leaves
 * Reader -> waits for bytes to read
 */
static inline void finilize(PCP_Guard* guard, Circular_Buffer* buffer);

static void print_usage(double array[static 1]);

void* thread_printer(void* printer_arguments) {

    if (printer_arguments == NULL) {
        return NULL;
    }

    enum {
        temp_buffer_size = 200,
    };

    Circular_Buffer* buffer;
    volatile bool* is_working;
    pthread_mutex_t* working_mutex;
    PCP_Guard* guard;
    double temp_buffer[temp_buffer_size] = {0};
    size_t index = 0;

    {
        thread_printer_arguments* temp = printer_arguments;

        buffer = temp->circular_buffer;
        is_working = temp->is_working;
        working_mutex = temp->working_mutex;
        guard = temp->circular_buffer_guard;
    }

    while(true) {
        pthread_mutex_lock(working_mutex); 
        if (!*is_working) {
            finilize(guard, buffer);
            pthread_mutex_unlock(working_mutex);
            break;
        }
        pthread_mutex_unlock(working_mutex);

        double temp_double;
        if (pcp_guard_lock(guard) != 0) {
            perror("Lock error: printer\n");
            continue;
        }
        if (circular_buffer_remove_single(buffer, &temp_double) == 0) {
            pcp_guard_wait_for_producer(guard);
            circular_buffer_remove_single(buffer, &temp_double);
        }
        pcp_guard_notify_producer(guard);
        pcp_guard_unlock(guard);

        temp_buffer[index++] = temp_double;
        if (temp_double == THREAD_PARSER_END) {
            print_usage(temp_buffer);
            index = 0;
        }
        if (index == temp_buffer_size) {
            perror("Buffer size is to small\n");
        }
    }
    return NULL;
}

static void print_usage(double array[static 1]) {
    size_t index = 0;
    while (array[index] != THREAD_PARSER_END) {
        printf("Core #%zu usage: %.2F%%\n", index, array[index]);
        index++;
    }
}

static inline void finilize(PCP_Guard* guard, Circular_Buffer* buffer) {
    /*The lock on buffer guard*/
    pcp_guard_lock(guard);
    double temp;
    /*Removing single element 
    NOTE: nothing will happend if buffer is empty*/
    circular_buffer_remove_single(buffer, &temp);
    /*Set parser free if he is currently locked*/
    pcp_guard_notify_producer(guard);
    /*unlock buffer guard*/
    pcp_guard_unlock(guard);
}

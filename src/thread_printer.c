#include <pthread.h>
#include <unistd.h>
#include "thread_printer.h"
#include "thread_parser.h"

static void print_usage(double array[static 1]);

void* thread_printer(void* printer_arguments) {


    enum {
        temp_buffer_size = 200,
    };


    Circular_Buffer* double_buffer = NULL;
    Circular_Buffer* logger_buffer = NULL;
    PCP_Guard* double_buffer_guard = NULL;
    PCP_Guard* logger_guard = NULL;
    Watchdog_Control_Unit* control_unit = NULL;
    bool* working = NULL;
    pthread_mutex_t* working_mutex = NULL;
    double temp_buffer[temp_buffer_size] = {0};
    size_t index = 0;

    {
        thread_printer_arguments* temp = printer_arguments;

        double_buffer = temp->circular_buffer;
        logger_buffer = temp->logger_buffer;
        double_buffer_guard = temp->circular_buffer_guard;
        logger_guard = temp->logger_buffer_guard;
        control_unit = temp->control_unit;
        working = temp->is_working;
        working_mutex = temp->working_mutex;

    }

    if (double_buffer == NULL || logger_buffer == NULL || double_buffer_guard == NULL 
        || logger_guard == NULL || working == NULL || working_mutex == NULL || control_unit == NULL) {
        perror("Printer: one of arguments equal to NULL\n");
        return NULL;
    }

    while(true) {
        pthread_mutex_lock(working_mutex); 
        if (!*working) {
            finilize(double_buffer_guard, double_buffer);
            pthread_mutex_unlock(working_mutex);
            watchdog_unit_atomic_finish(control_unit);
            break;
        }
        pthread_mutex_unlock(working_mutex);

        double temp_double;
        pcp_guard_lock(double_buffer_guard);
        if (circular_buffer_remove_single(double_buffer, &temp_double) == 0) {
            pcp_guard_wait_for_producer(double_buffer_guard);
            circular_buffer_remove_single(double_buffer, &temp_double);
        }
        pcp_guard_notify_producer(double_buffer_guard);
        pcp_guard_unlock(double_buffer_guard);

        temp_buffer[index++] = temp_double;
        if (temp_double == THREAD_PARSER_END) {
            print_usage(temp_buffer);
            index = 0;
        }
        if (index == temp_buffer_size) {

            thread_logger_send_log(logger_guard, logger_buffer,
            "Buffer size is to small\n", LOGGER_PAYLOAD_TYPE_WARNING);
        }
        watchdog_unit_atomic_ping(control_unit);
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


static inline void finilize(PCP_Guard* double_buffer_guard, Circular_Buffer* double_buffer) {
    /*The lock on buffer guard*/
    pcp_guard_lock(double_buffer_guard);
    double temp;
    /*Removing single element 
    NOTE: nothing will happend if buffer is empty*/
    circular_buffer_remove_single(double_buffer, &temp);
    /*Set parser free if he is currently locked*/
    pcp_guard_notify_producer(double_buffer_guard);
    /*unlock buffer guard*/
    pcp_guard_unlock(double_buffer_guard);
}

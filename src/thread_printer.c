#include <pthread.h>
#include <unistd.h>
#include "thread_printer.h"

static inline void finilize(PCP_Guard* guard, Circular_Buffer* buffer);
static void print_usage(double array[static 1]);

void* thread_printer(void* printer_arguments) {

    Circular_Buffer* buffer;
    volatile bool* is_working;
    pthread_mutex_t* working_mutex;
    PCP_Guard* guard;
    double temp_buffer[200] = {0};
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
        if (temp_double == -1) {
            print_usage(temp_buffer);
            index = 0;
        }
    }
    return NULL;
}

static void print_usage(double array[static 1]) {
    size_t index = 0;
    while (array[index] != -1.0) {
        printf("Core #%zu usage: %.2F%%\n", index, array[index] * 100);
        index++;
    }
}

static inline void finilize(PCP_Guard* guard, Circular_Buffer* buffer) {

    pcp_guard_lock(guard);
    double temp;
    circular_buffer_remove_single(buffer, &temp);
    pcp_guard_notify_producer(guard);
    pcp_guard_unlock(guard);
}

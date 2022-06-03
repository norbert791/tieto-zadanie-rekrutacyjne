#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include "thread_reader.h"
#include "circular_buffer.h"
#include "pcp_guard.h"


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
static inline void finilize(Circular_Buffer* buffer, PCP_Guard* guard);

void* thread_reader(void* reader_aguments) {

    /*Sanity check*/
    if (reader_aguments == NULL) {
        /*Error handling*/
        return NULL;
    }

    const struct timespec sleep_time = {.tv_nsec = 0, .tv_sec = 1};
    PCP_Guard* guard;
    Circular_Buffer* buffer;
    volatile bool* is_working;
    pthread_mutex_t* working_mtx;
    FILE* input_file;

    {
        thread_reader_arguments* temp = reader_aguments;

        guard = temp->circular_buffer_guard;
        buffer = temp->circular_buffer;
        is_working = temp->is_working;
        working_mtx = temp->working_mutex;
        input_file = temp->input_file;

        temp = NULL;
    }

    /*Sanity check*/
    if (buffer == NULL || is_working == NULL || working_mtx == NULL || input_file == NULL || guard == NULL) {
        /*Error handling*/
        return NULL;
    }

    while (true) {
        pthread_mutex_lock(working_mtx);
        if (!*is_working) {
            finilize(buffer, guard);
            pthread_mutex_unlock(working_mtx);
            break;
        }
        
        pthread_mutex_unlock(working_mtx);
        char input_char = fgetc(input_file);
       
        if (input_char != EOF) {
            pcp_guard_lock(guard);

            if (circular_buffer_insert_single(buffer, &input_char) == 0) {
                pcp_guard_wait_for_consumer(guard);
                circular_buffer_insert_single(buffer, &input_char);
            }
            pcp_guard_notify_consumer(guard);
            pcp_guard_unlock(guard);
        }
        else if (feof(input_file)) {
            if (nanosleep(&sleep_time, NULL) != 0) {
                errno = 0;
                perror("Sleep error\n");
            }
            fflush(input_file);
            rewind(input_file);
        }
        else if (ferror(input_file)) {
            /*Error handling*/
            perror("File error during read atempt\n");
        }
    }
    return NULL;
}

static inline void finilize(Circular_Buffer* buffer, PCP_Guard* guard) {
    /*lock on buffer guard*/
    pcp_guard_lock(guard);
    /*Insert some garbage that will be discarded anyway, 
    NOTE: if buffer is full, nothing will happen*/
    const char temp = ' ';
    circular_buffer_insert_single(buffer, &temp);
    /*If reader is lock on buffer guard, then it will be released after unlock*/
    pcp_guard_notify_consumer(guard);
    /*Relase buffer guard*/
    pcp_guard_unlock(guard);
}

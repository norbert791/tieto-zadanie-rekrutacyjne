#include <stdbool.h>
#include <pthread.h>
#include "logger.h"
#include "thread_reader.h"
#include "circular_buffer.h"
#include "pcp_guard.h"


static inline void finilize(Circular_Buffer* buffer, PCP_Guard* guard);

void* thread_reader(void* reader_aguments) {

    /*Sanity check*/
    if (reader_aguments == NULL) {
        /*Error handling*/
        return NULL;
    }

    PCP_Guard* guard;
    Circular_Buffer* buffer;
    bool* is_working;
    pthread_mutex_t* working_mtx;
    FILE* input_file;

    {
        thread_reader_arguments* temp = reader_aguments;

        guard = temp->Circular_Buffer_guard;
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
        if (!is_working) {
            finilize(buffer, guard);
            pthread_mutex_unlock(working_mtx);
            break;
        }
        pthread_mutex_unlock(working_mtx);
        char input_char = fgetc(input_file);
        if (input_char != EOF) {
            if (pcp_guard_lock(buffer) != 0) {
                perror("Lock error\n");
            }

            if (circular_buffer_insert_single(buffer, &input_char) == 0) {
                pcp_guard_wait_for_consumer(guard);
                circular_buffer_insert_single(buffer, &input_char);
            }
            pcp_guard_notify_consumer(guard);
            pcp_guard_unlock(guard);
        }
        else if (feof(input_file)) {
            rewind(input_file);
        }
        else if (ferror(input_file)) {
            /*Error handling*/
            perror("File error during read atempt\n");
        }
    }

}


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
static inline void finilize(Circular_Buffer* buffer, PCP_Guard* guard) {
    /*lock on buffer */
    pcp_guard_lock(guard);
    /*Insert some garbage that will be discarded anyway, 
    NOTE: if buffer is full, nothing will happen*/
    const char temp = ' ';
    circular_buffer_insert_single(buffer, &temp);
    /*Notify reader. It will lock either lock on is_working or on buffer */
    pcp_guard_notify_consumer(guard);
    /*Relase buffer */
    pcp_guard_unlock(guard);
}
#include "thread_parser.h"
#include "proc_parser.h"
#include "pcp_guard.h"

static inline void finilize_read(Circular_Buffer* char_buffer, PCP_Guard* guard);
static inline void finilize_write(Circular_Buffer* char_buffer, PCP_Guard* guard);

static double parse_lien(char buffer[static 1]);

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
    bool* is_working;
    pthread_mutex_t* working_mtx;

    size_t index = 0;
    /*TODO: find if there is upper limit for line width in proc/stat*/
    const size_t max_size = 400;
    char temporary_buffer[max_size];


    {
        thread_parser_arguments* temp = args;

        char_buffer = temp->char_buffer;
        double_buffer = temp->double_buffer;
        char_buffer_guard = temp->char_buffer_guard;
        double_buffer_guard = temp->char_buffer_guard;
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
        if (!is_working) {
            finilize_read(char_buffer, char_buffer_guard);
            finilize_write(double_buffer, double_buffer_guard);
            pthread_mutex_unlock(working_mtx);
            break;
        }
        if (Synchronized_Circular_Buffer_lock(char_buffer) != 0) {
            /*Error handling*/
        }
        Circular_Buffer* temp = Synchronized_Circular_Buffer_get(char_buffer);

        char input_char;
        if (Circular_Buffer_remove_single(temp, input_char) == 0) {
            Synchronized_Circular_Buffer_wait_for_writer(char_buffer);
            Circular_Buffer_remove_single(temp, input_char);   
        }
        Synchronized_Circular_Buffer_notify_writer(char_buffer);
        
        if (input_char == '\n') {
            temporary_buffer[index] = '\0';
            index = 0;

        }
        else {
            temporary_buffer[index] = input_char;
            index++;
            if (index == max_size) {
                /*Line width is larger than buffer - handle it accordingly */
                index = 0;
            }
           // proc_parser_parse_line(temporary_buffer, )
        }

    }

    return NULL;
}


static inline void finilize_read(Circular_Buffer* char_buffer, PCP_Guard* guard) {
    /*lock on buffer */
    pcp_guard_lock(char_buffer);
    /*Insert some garbage that will be discarded anyway, 
    NOTE: if buffer is full, nothing will happen*/
    char temp;
    circular_buffer_remove_single(temp, &temp);
    /*Notify reader. It will lock either lock on is_working or on buffer */
    pcp_guard_notify_producer(char_buffer);
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
    Circular_Buffer_insert_single(buffer, &d);
    /*Notify reader. It will lock either lock on is_working or on buffer */
    pcp_guard_notify_consumer(guard);
    /*Relase buffer */
    pcp_guard_unlock(guard);
}
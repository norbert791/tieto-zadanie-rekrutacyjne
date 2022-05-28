#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include "circular_buffer.h"
#include "thread_reader.h"
#include "thread_parser.h"
#include "thread_printer.h"

PCP_Guard char_buffer_guard;
PCP_Guard double_buffer_guard;

Circular_Buffer* reader_char_buffer;
Circular_Buffer* parser_double_buffer;

FILE* proc_file;
bool is_working = true;
pthread_mutex_t is_working_mutex = PTHREAD_MUTEX_INITIALIZER;

static void resources_release();
static bool resources_initialization();
static bool threads_initialization();
static void threads_join();
static void term_handler(int var);

pthread_t reader_thread;
pthread_t parser_thread;
pthread_t printer_thread;

thread_reader_arguments reader_args;
thread_parser_arguments parser_args;
thread_printer_arguments printer_args;

int main() {

    if (!resources_initialization()) {
        return EXIT_FAILURE;
    }
 /*   
    struct sigaction action;
    action.sa_handler = term_handler;

    if (sigaction(SIGTERM, &action, NULL) == -1) {
        errno = 0;
        perror("Sigaction error\n");
        resources_release();
        return EXIT_FAILURE;
    }*/

    if (!threads_initialization()) {
        perror("Threads setup failed");
        return EXIT_FAILURE;
    }
    
    threads_join();
    resources_release();

    return 0;
}


static void threads_join() {
    pthread_join(reader_thread, NULL);
    pthread_join(parser_thread, NULL);
    pthread_join(printer_thread, NULL);
}

static bool threads_initialization() {

    reader_args.circular_buffer_guard = &char_buffer_guard;
    reader_args.circular_buffer = reader_char_buffer;
    reader_args.input_file = proc_file;
    reader_args.is_working = &is_working;
    reader_args.working_mutex = &is_working_mutex;

    parser_args.char_buffer = reader_char_buffer;
    parser_args.double_buffer = parser_double_buffer;
    parser_args.char_buffer_guard = &char_buffer_guard;
    parser_args.double_buffer_guard = &double_buffer_guard;
    parser_args.is_working = &is_working;
    parser_args.working_mutex = &is_working_mutex;

    printer_args.circular_buffer_guard = &double_buffer_guard;
    printer_args.circular_buffer = parser_double_buffer;
    printer_args.is_working = &is_working;
    printer_args.working_mutex = &is_working_mutex;
    
    if (pthread_create(&reader_thread, NULL, thread_reader, &reader_args) != 0) {
        perror("Thread creation error \n");
        return false;
    }
    if (pthread_create(&parser_thread, NULL, thread_parser, &parser_args) != 0) {
        perror("Thread creation error \n");
        pthread_join(reader_thread, NULL);
        return false;
    }
    if (pthread_create(&printer_thread, NULL, thread_printer, &printer_args) != 0) {
        perror("Thread creation error \n");
        pthread_join(reader_thread, NULL);
        pthread_join(parser_thread, NULL);
        return false;
    }
    return true;
}

static void term_handler(int var) {
    pthread_mutex_lock(&is_working_mutex);
    is_working = false;
    pthread_mutex_unlock(&is_working_mutex);
}

static void resources_release() {

    fclose(proc_file);
    circular_buffer_delete(reader_char_buffer);
    circular_buffer_delete(parser_double_buffer);
    pthread_mutex_destroy(&is_working_mutex);
    pcp_guard_destroy(&char_buffer_guard);
    pcp_guard_destroy(&double_buffer_guard);

}

static bool resources_initialization() {
    /*Initializations*/
    reader_char_buffer = circular_buffer_new(400, sizeof(char));
    if (reader_char_buffer == NULL) {
        perror("Program initialization failed: memory error\n");
        pthread_mutex_destroy(&is_working_mutex);
        return false;
    }
    
    parser_double_buffer = circular_buffer_new(32, sizeof(double));

    if (parser_double_buffer == NULL) {
        perror("Program initialization failed: memory error\n");
        circular_buffer_delete(reader_char_buffer);
        pthread_mutex_destroy(&is_working_mutex);
        return false;
    }

    if (pcp_guard_init(&char_buffer_guard) != PCP_SUCCESS) {
        perror("Program initialization failed: synchronization error\n");
        errno = 0;
        circular_buffer_delete(reader_char_buffer);
        circular_buffer_delete(parser_double_buffer);
        pthread_mutex_destroy(&is_working_mutex);
        return false;
    }

    if (pcp_guard_init(&double_buffer_guard) != PCP_SUCCESS) {
        perror("Program initialization failed: synchronization error\n");
        errno = 0;
        circular_buffer_delete(reader_char_buffer);
        circular_buffer_delete(parser_double_buffer);
        pcp_guard_destroy(&char_buffer_guard);
        pthread_mutex_destroy(&is_working_mutex);
        return false;
    }
    proc_file = fopen("/proc/stat", "r");
    if (proc_file == NULL) {
        errno = 0;
        perror("IO error\n");
        circular_buffer_delete(reader_char_buffer);
        circular_buffer_delete(parser_double_buffer);
        pcp_guard_destroy(&char_buffer_guard);
        pcp_guard_destroy(&double_buffer_guard);
        pthread_mutex_destroy(&is_working_mutex);
        return false;
    }
    /*setting full buffering*/
    setvbuf(proc_file, NULL, _IOFBF, 1);
    return true;
}
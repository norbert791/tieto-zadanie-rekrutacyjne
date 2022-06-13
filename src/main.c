#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "circular_buffer.h"
#include "thread_reader.h"
#include "thread_parser.h"
#include "thread_printer.h"
#include "thread_watchdog.h"
#include "thread_logger.h"


static PCPGuard char_buffer_guard = PCP_GUARD_INITIALIZER, double_buffer_guard =  PCP_GUARD_INITIALIZER, logger_buffer_guard = PCP_GUARD_INITIALIZER;
static pthread_mutex_t working_mutex = PTHREAD_MUTEX_INITIALIZER;
static WatchdogControlUnit reader_unit =  WATCHDOG_CONTROL_UNIT_INIT, parser_unit = WATCHDOG_CONTROL_UNIT_INIT,
                                        printer_unit = WATCHDOG_CONTROL_UNIT_INIT, logger_unit = WATCHDOG_CONTROL_UNIT_INIT;

static FILE* proc_file;
static FILE* logger_file;
static CircularBuffer* char_buffer;
static CircularBuffer* double_buffer;
static CircularBuffer* logger_buffer;
static Watchdog* watchdog;

static pthread_t watchdog_id = 0;
static bool working = true;
static volatile sig_atomic_t stop_condition = 1;

static ThreadReaderArguments reader_args;
static ThreadParserArguments parser_args;
static ThreadPrinterArguments printer_args;
static ThreadWatchdogArguments watchdog_args;
static ThreadLoggerArguments logger_args;

static inline void resources_release();
static inline bool resource_initialization();
static inline bool threads_initialization();
static inline void threads_join();
static void term_handler(int sigterm);

int main() {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGINT);

    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = term_handler;
    if (sigaction(SIGTERM, &action, NULL) == -1) {
        errno = 0;
        perror("Sigaction error\n");
        return EXIT_FAILURE;
    }
    if (sigaction(SIGINT, &action, NULL) == -1) {
        errno = 0;
        perror("Sigaction error\n");
        return EXIT_FAILURE;
    }
    if (pthread_sigmask(SIG_BLOCK, &mask, NULL) != 0) {
        perror("Failed to set mask\n");
        return EXIT_FAILURE;
    }

    if (!resource_initialization()) {
        perror("Resource initialization failed\n");
        return EXIT_FAILURE;
    }
    if (!threads_initialization()) {
        perror("Threads setup failed\n");
        resources_release();
        return EXIT_FAILURE;
    }
    if (pthread_sigmask(SIG_UNBLOCK, &mask, NULL) != 0) {
        perror("Failed to set mask\n");
        stop_condition = 0;
    }
    
    while (stop_condition > 0) {
        pause();
        errno = 0;
    }
    pthread_mutex_lock(&working_mutex);
    working = false;
    pthread_mutex_unlock(&working_mutex);
    threads_join();
    resources_release();

    return 0;
}

static inline bool resource_initialization() {

    char_buffer = circular_buffer_new(400, sizeof(char));
    if (char_buffer == NULL) {
        perror("Initialization failed: memory error\n");
        return false;
    }

    double_buffer = circular_buffer_new(128, sizeof(double));
    if (double_buffer == NULL) {
        circular_buffer_delete(char_buffer);
        return false;
    }
    
    logger_buffer = circular_buffer_new(50, sizeof(void*));
    if (logger_buffer == NULL) {
        circular_buffer_delete(char_buffer);
        circular_buffer_delete(double_buffer);
        return false;
    }

    watchdog = watchdog_new(4);
    if (watchdog == NULL) {
        circular_buffer_delete(char_buffer);
        circular_buffer_delete(double_buffer);
        circular_buffer_delete(logger_buffer);
        return false;
    }

    proc_file = fopen("/proc/stat", "rb");
    if (proc_file == NULL) {
        errno = 0;
        perror("IO error\n");
        circular_buffer_delete(char_buffer);
        circular_buffer_delete(double_buffer);
        circular_buffer_delete(logger_buffer);
        return false;
    }

    logger_file = fopen("program_log.txt", "w");
    if (logger_file == NULL) {
        errno = 0;
        perror("IO error\n");
        circular_buffer_delete(char_buffer);
        circular_buffer_delete(double_buffer);
        circular_buffer_delete(logger_buffer);
        return false;
    }

    setvbuf(proc_file, NULL, _IOFBF, 1);
    return true;
}

static inline void resources_release() {

    circular_buffer_delete(char_buffer);
    circular_buffer_delete(double_buffer);

    LoggerPayload* temp = NULL;
    while (circular_buffer_remove_single(logger_buffer, &temp) > 0) {
        logger_payload_delete(temp);
    }
    temp = NULL;
    circular_buffer_delete(logger_buffer);
    watchdog_delete(watchdog);
    watchdog = NULL;
    fclose(proc_file);
    fclose(logger_file);

    pcp_guard_destroy(&char_buffer_guard);
    pcp_guard_destroy(&double_buffer_guard);
    pcp_guard_destroy(&logger_buffer_guard);
    
    watchdog_unit_destroy(&reader_unit);
    watchdog_unit_destroy(&parser_unit);
    watchdog_unit_destroy(&printer_unit);
    watchdog_unit_destroy(&logger_unit);
    
    pthread_mutex_destroy(&working_mutex);
}

static inline bool threads_initialization() {

    reader_args.char_buffer = char_buffer;
    reader_args.char_buffer_guard = &char_buffer_guard;
    reader_args.control_unit = &reader_unit;
    reader_args.input_file = proc_file;
    reader_args.logger_buffer = logger_buffer;
    reader_args.logger_buffer_guard = &logger_buffer_guard;
    reader_args.working = &working;
    reader_args.working_mutex = &working_mutex;
    
    parser_args.char_buffer = char_buffer;
    parser_args.char_buffer_guard = &char_buffer_guard;
    parser_args.control_unit = &parser_unit;
    parser_args.double_buffer = double_buffer;
    parser_args.double_buffer_guard = &double_buffer_guard;
    parser_args.is_working = &working;
    parser_args.logger_buffer = logger_buffer;
    parser_args.logger_buffer_guard = &logger_buffer_guard;
    parser_args.working_mutex = &working_mutex;

    printer_args.circular_buffer = double_buffer;
    printer_args.circular_buffer_guard = &double_buffer_guard;
    printer_args.control_unit = &printer_unit;
    printer_args.is_working = &working;
    printer_args.logger_buffer = logger_buffer;
    printer_args.logger_buffer_guard = &logger_buffer_guard;
    printer_args.working_mutex = &working_mutex;

    logger_args.buffer_guard = &logger_buffer_guard;
    logger_args.control_unit = &logger_unit;
    logger_args.is_working = &working;
    logger_args.is_working_mutex = &working_mutex;
    logger_args.logger_output = logger_file;
    logger_args.logger_payload_pointer_buffer = logger_buffer;

    watchdog_args.is_working = &working;
    watchdog_args.mutex = &working_mutex;
    watchdog_args.watchdog = watchdog;

    


    if (pthread_create(&logger_unit.thread_id, NULL, thread_logger, &logger_args) != 0) {
        perror("logger creation error \n");
        pthread_mutex_lock(&working_mutex);
        working = false;
        pthread_mutex_unlock(&working_mutex);
        return false;
    }
    if (pthread_create(&reader_unit.thread_id, NULL, thread_reader, &reader_args) != 0) {
        perror("Thread reader creation error\n");
        pthread_join(logger_unit.thread_id, NULL);
        return false;
    }
    if (pthread_create(&parser_unit.thread_id, NULL, thread_parser, &parser_args) != 0) {
        perror("Thread parser creation error \n");
        pthread_mutex_lock(&working_mutex);
        working = false;
        pthread_mutex_unlock(&working_mutex);
        pthread_join(reader_unit.thread_id, NULL);
        pthread_join(logger_unit.thread_id, NULL);

        return false;
    }
    if (pthread_create(&printer_unit.thread_id, NULL, thread_printer, &printer_args) != 0) {
        perror("Thread printer creation error \n");
        pthread_mutex_lock(&working_mutex);
        working = false;
        pthread_mutex_unlock(&working_mutex);
        pthread_join(reader_unit.thread_id, NULL);
        pthread_join(parser_unit.thread_id, NULL);
        pthread_join(printer_unit.thread_id, NULL);
        return false;
    }
    
    watchdog_add_puppy(watchdog, &reader_unit);
    watchdog_add_puppy(watchdog, &parser_unit);
    watchdog_add_puppy(watchdog, &printer_unit);
    watchdog_add_puppy(watchdog, &logger_unit);

    if (pthread_create(&watchdog_id, NULL, thread_watchdog, &watchdog_args) != 0) {
        perror("Thread creation error \n");
        pthread_mutex_lock(&working_mutex);
        working = false;
        pthread_mutex_unlock(&working_mutex);
        pthread_join(reader_unit.thread_id, NULL);
        pthread_join(parser_unit.thread_id, NULL);
        pthread_join(printer_unit.thread_id, NULL);
        pthread_join(logger_unit.thread_id, NULL);
        return false;
    }
    return true;
}

static inline void threads_join() {
    pthread_join(reader_unit.thread_id, NULL);
    pthread_join(parser_unit.thread_id, NULL);
    pthread_join(printer_unit.thread_id, NULL);
    pthread_join(logger_unit.thread_id, NULL);
    pthread_join(watchdog_id, NULL);
}

static void term_handler(int signum) {
    if (signum == SIGTERM || signum == SIGINT) {
        stop_condition = 0;
    }
}

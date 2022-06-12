#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
#include "circular_buffer.h"
#include "thread_reader.h"
#include "thread_parser.h"
#include "thread_printer.h"
#include "thread_watchdog.h"
#include "thread_logger.h"


static PCPGuard guard[3] = {PCP_GUARD_INITIALIZER, PCP_GUARD_INITIALIZER, PCP_GUARD_INITIALIZER};
static pthread_mutex_t working_mutex = PTHREAD_MUTEX_INITIALIZER;
static WatchdogControlUnit control_unit[4] = {WATCHDOG_CONTROL_UNIT_INIT, WATCHDOG_CONTROL_UNIT_INIT,
                                                WATCHDOG_CONTROL_UNIT_INIT, WATCHDOG_CONTROL_UNIT_INIT};
static pthread_t watchdog_id = 0;
static bool working = true;
static volatile sig_atomic_t stop_condition = 1;

static ThreadReaderArguments reader_args;
static ThreadParserArguments parser_args;
static ThreadPrinterArguments printer_args;
static ThreadWatchdogArguments watchdog_args;
static ThreadLoggerArguments logger_args;

static inline void resources_release(CircularBuffer* restrict buffers[restrict static 3],
                                            FILE* restrict files[restrict static 2], Watchdog** restrict watchdog);
static inline bool resource_initialization(CircularBuffer* restrict buffers[restrict static 3],
                                            FILE* restrict files[restrict static 2], Watchdog** restrict watchdog);
static inline bool threads_initialization(CircularBuffer* restrict buffers[restrict static 3],
                                            FILE* restrict files[restrict static 2], Watchdog* restrict watchdog);
static inline void threads_join();
static void term_handler(int var);

int main() {

    FILE* files[2] = {0};
    CircularBuffer* buffers[3] = {0};
    Watchdog* watchdog = NULL;
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

    if (!resource_initialization(buffers, files, &watchdog)) {
        perror("Resource initialization failed\n");
        return EXIT_FAILURE;
    }
    if (!threads_initialization(buffers, files, watchdog)) {
        perror("Threads setup failed\n");
        resources_release(buffers, files, &watchdog);
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
    resources_release(buffers, files, &watchdog);

    return 0;
}

static inline bool resource_initialization(CircularBuffer* restrict buffers[restrict static 3],
                                            FILE* restrict files[restrict static 2], Watchdog** watchdog) {

    /*char_buffer*/
    buffers[0] = circular_buffer_new(400, sizeof(char));
    if (buffers[0] == NULL) {
        perror("Initialization failed: memory error\n");
        return false;
    }
    /*double_buffer*/

    buffers[1] = circular_buffer_new(128, sizeof(double));
    if (buffers[1] == NULL) {
        circular_buffer_delete(buffers[0]);
        return false;
    }
    
    buffers[2] = circular_buffer_new(50, sizeof(void*));
    if (buffers[2] == NULL) {
        circular_buffer_delete(buffers[0]);
        circular_buffer_delete(buffers[1]);
        return false;
    }

    *watchdog = watchdog_new(4);
    if (*watchdog == NULL) {
        circular_buffer_delete(buffers[0]);
        circular_buffer_delete(buffers[1]);
        circular_buffer_delete(buffers[2]);
        return false;
    }

    files[0] = fopen("/proc/stat", "rb");
    if (files[0] == NULL) {
        errno = 0;
        perror("IO error\n");
        circular_buffer_delete(buffers[0]);
        circular_buffer_delete(buffers[1]);
        circular_buffer_delete(buffers[2]);
        return false;
    }

    files[1] = fopen("program_log.txt", "w");
    if (files[1] == NULL) {
        errno = 0;
        perror("IO error\n");
        circular_buffer_delete(buffers[0]);
        circular_buffer_delete(buffers[1]);
        circular_buffer_delete(buffers[2]);
        return false;
    }

    setvbuf(files[0], NULL, _IOFBF, 1);
    setvbuf(files[1], NULL, _IOFBF, 1);
    return true;
}

static inline void resources_release(CircularBuffer* restrict buffers[restrict static 3],
                                            FILE* restrict files[restrict static 2], Watchdog** restrict watchdog) {

        circular_buffer_delete(buffers[0]);
        circular_buffer_delete(buffers[1]);
        
        LoggerPayload* temp = NULL;
        while (circular_buffer_remove_single(buffers[2], &temp) > 0) {
            logger_payload_delete(temp);
        }
        temp = NULL;
        circular_buffer_delete(buffers[2]);
        watchdog_delete(*watchdog);
        *watchdog = NULL;
        fclose(files[0]);
        fclose(files[1]);
}

static inline bool threads_initialization(CircularBuffer* restrict buffers[restrict static 3],
                                            FILE* restrict files[restrict static 2], Watchdog* restrict watchdog) {

    reader_args.char_buffer = buffers[0];
    reader_args.char_buffer_guard = &guard[0];
    reader_args.control_unit = &control_unit[0];
    reader_args.input_file = files[0];
    reader_args.logger_buffer = buffers[2];
    reader_args.logger_buffer_guard = &guard[2];
    reader_args.working = &working;
    reader_args.working_mutex = &working_mutex;
    
    parser_args.char_buffer = buffers[0];
    parser_args.char_buffer_guard = &guard[0];
    parser_args.control_unit = &control_unit[1];
    parser_args.double_buffer = buffers[1];
    parser_args.double_buffer_guard = &guard[1];
    parser_args.is_working = &working;
    parser_args.logger_buffer = buffers[2];
    parser_args.logger_buffer_guard = &guard[2];
    parser_args.working_mutex = &working_mutex;

    printer_args.circular_buffer = buffers[1];
    printer_args.circular_buffer_guard = &guard[1];
    printer_args.control_unit = &control_unit[2];
    printer_args.is_working = &working;
    printer_args.logger_buffer = buffers[2];
    printer_args.logger_buffer_guard = &guard[2];
    printer_args.working_mutex = &working_mutex;

    logger_args.buffer_guard = &guard[2];
    logger_args.control_unit = &control_unit[3];
    logger_args.is_working = &working;
    logger_args.is_working_mutex = &working_mutex;
    logger_args.logger_output = files[1];
    logger_args.logger_payload_pointer_buffer = buffers[2];

    watchdog_args.is_working = &working;
    watchdog_args.mutex = &working_mutex;
    watchdog_args.watchdog = watchdog;

    


    if (pthread_create(&control_unit[3].thread_id, NULL, thread_logger, &logger_args) != 0) {
        perror("logger creation error \n");
        pthread_mutex_lock(&working_mutex);
        working = false;
        pthread_mutex_unlock(&working_mutex);
        return false;
    }
    if (pthread_create(&control_unit[0].thread_id, NULL, thread_reader, &reader_args) != 0) {
        perror("Thread reader creation error\n");
        pthread_join(control_unit[3].thread_id, NULL);
        return false;
    }
    if (pthread_create(&control_unit[1].thread_id, NULL, thread_parser, &parser_args) != 0) {
        perror("Thread creation error \n");
        pthread_mutex_lock(&working_mutex);
        working = false;
        pthread_mutex_unlock(&working_mutex);
        pthread_join(control_unit[0].thread_id, NULL);
        pthread_join(control_unit[3].thread_id, NULL);

        return false;
    }
    if (pthread_create(&control_unit[2].thread_id, NULL, thread_printer, &printer_args) != 0) {
        perror("Thread creation error \n");
        pthread_mutex_lock(&working_mutex);
        working = false;
        pthread_mutex_unlock(&working_mutex);
        pthread_join(control_unit[0].thread_id, NULL);
        pthread_join(control_unit[1].thread_id, NULL);
        pthread_join(control_unit[3].thread_id, NULL);
        return false;
    }
    


    watchdog_add_puppy(watchdog, &control_unit[0]);
    watchdog_add_puppy(watchdog, &control_unit[1]);
    watchdog_add_puppy(watchdog, &control_unit[2]);
    watchdog_add_puppy(watchdog, &control_unit[3]);

    if (pthread_create(&watchdog_id, NULL, thread_watchdog, &watchdog_args) != 0) {
        perror("Thread creation error \n");
        pthread_mutex_lock(&working_mutex);
        working = false;
        pthread_mutex_unlock(&working_mutex);
        pthread_join(control_unit[0].thread_id, NULL);
        pthread_join(control_unit[1].thread_id, NULL);
        pthread_join(control_unit[2].thread_id, NULL);
        pthread_join(control_unit[3].thread_id, NULL);
        return false;
    }
    return true;
}

static inline void threads_join() {
    //puts("reader\n");
    pthread_join(control_unit[0].thread_id, NULL);
   // puts("parser\n");
    pthread_join(control_unit[1].thread_id, NULL);
   // puts("printer\n");
    pthread_join(control_unit[2].thread_id, NULL);
  //  puts("logger\n");
    pthread_join(control_unit[3].thread_id, NULL);
  //  puts("watchdog\n");
    pthread_join(watchdog_id, NULL);
}

static void term_handler(int var) {
    stop_condition = 0;
}
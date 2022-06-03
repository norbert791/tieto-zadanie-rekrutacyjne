#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "proc_parser.h"


int proc_parser_parse_line(const char buffer[const static 5], uint64_t result[static 10]) {

    static const char* specifiers = "%*s" " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64;
    
    if (strncmp(buffer, "cpu", 3) != 0) {
        return PROC_PARSER_DISCARD_LINE;
    }
    else if (isdigit(buffer[3]) == 0) {
        return PROC_PARSER_TOTAL_USAGE_LINE;
    }
    
    int symbols_read = sscanf(buffer, specifiers, &result[0], &result[1], &result[2], &result[3], &result[4], &result[5], &result[6], &result[7], &result[8], &result[9]);

    if (symbols_read == EOF) {
        return PROC_PARSER_FAIL;
    }

    return symbols_read;
}

double proc_parser_compute_core_usage(const uint64_t previous_values[const static 10], const uint64_t current_values[const static 10]) {

    double prev_idle = previous_values[3] + previous_values[4];
    double idle = current_values[3] + current_values[4];

    double prev_non_idle = previous_values[0] + previous_values[1] + previous_values[2] + previous_values[5] + previous_values[6] + previous_values[7];
    double non_idle = current_values[0] + current_values[1] + current_values[2] + current_values[5] + current_values[6] + current_values[7];
    
    double prev_total = prev_idle + prev_non_idle;
    double total = idle + non_idle;

    double total_delta = total - prev_total > 0.0 ? total - prev_total : 1.0;
    double idle_d = idle - prev_idle;

    return (total_delta - idle_d) / total_delta;
}

proc_parser_cpu_time proc_parser_compute_core_time(const uint64_t core_line[static 10]) {
    uint64_t idle = core_line[3] + core_line[4];
    uint64_t non_idle = core_line[0] + core_line[1] + core_line[2] + core_line[5] + core_line[6] + core_line[7];

    uint64_t total = idle + non_idle;

    return (proc_parser_cpu_time) {.total = total, .idle = idle};;
}

double proc_parser_cpu_time_compute_usage(const proc_parser_cpu_time* previous, const proc_parser_cpu_time* current) {

    double total_delta = current->total - previous->total;
    double idle_delta = current->idle - previous->idle;
    total_delta = total_delta > 0.0 ? total_delta : idle_delta;

    return (total_delta - idle_delta) / total_delta; 

}
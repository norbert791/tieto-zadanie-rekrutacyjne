#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "proc_parser.h"


int proc_parser_parse_line(const char buffer[const static 5], uint64_t result[static 10]) {

    static const char* specifiers = "%*s" " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64;
    
    if (strncmp(buffer, "cpu", 3) != 0) {
        return -2;
    }
    else if (isdigit(buffer[3]) == 0) {
        return -1;
    }
    
    int symbols_read = sscanf(buffer, specifiers, &result[0], &result[1], &result[2], &result[3], &result[4], &result[5], &result[6], &result[7], &result[8], &result[9]);

    if (symbols_read == EOF) {
        return -3;
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

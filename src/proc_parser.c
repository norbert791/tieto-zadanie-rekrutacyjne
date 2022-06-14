#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "proc_parser.h"


int proc_parser_parse_line(const char buffer[const restrict static 5], uint64_t result[const restrict static 10]) {
    
    if (strncmp(buffer, "cpu", 3) != 0) {
        return PROC_PARSER_DISCARD_LINE;
    }
    else if (isdigit(buffer[3]) == 0) {
        return PROC_PARSER_TOTAL_USAGE_LINE;
    }
    
    int symbols_read = sscanf(buffer, "%*s" " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 
    " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64, &result[0], &result[1], &result[2], &result[3], &result[4],
                              &result[5], &result[6], &result[7], &result[8], &result[9]);

    if (symbols_read == EOF) {
        return PROC_PARSER_FAIL;
    }

    return symbols_read;
}

ProcParserCpuTime proc_parser_compute_core_time(const uint64_t core_line[const static 10]) {
    uint64_t idle = core_line[3] + core_line[4];
    uint64_t non_idle = core_line[0] + core_line[1] + core_line[2] + core_line[5] + core_line[6] + core_line[7];

    uint64_t total = idle + non_idle;

    return (ProcParserCpuTime) {.total = total, .idle = idle};
}

double proc_parser_cpu_time_compute_usage(const ProcParserCpuTime* const previous, const ProcParserCpuTime* const current) {
    double total_delta = (double)(current->total - previous->total);
    double idle_delta = (double)(current->idle - previous->idle);
    total_delta = total_delta > 0.0 ? total_delta : idle_delta;

    return (total_delta - idle_delta) / total_delta; 
}

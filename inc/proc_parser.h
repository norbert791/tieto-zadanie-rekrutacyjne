/**
 * @file proc_parser.h
 * @brief Support functions for parsing proc/stat data
 * 
 */
#ifndef PROC_PARSER_H
#define PROC_PARSER_H

#include <inttypes.h>

typedef enum EProcParserResult {
    PROC_PARSER_TOTAL_USAGE_LINE = -1,
    PROC_PARSER_DISCARD_LINE = -2,
    PROC_PARSER_FAIL = -3,
    PROC_PARSER_SUCCESS = 10,
} EProcParserResult;

typedef struct ProcParserCpuTime {
    uint64_t total;
    uint64_t idle;
} ProcParserCpuTime;

/**
 * @brief Parse /proc/stat line contained in buffer and insert integers representing
 * retrieved data into result array in the same order they appear in /proc/stat
 * 
 * If buffer does not contain line starting with 'cpu?' result will not be altered
 * 
 * @param buffer null-terminated string with at least 4 characters
 * @param result array for storing result with space for at least 10 elements
 * @return if line started with 'cpu[0-9]' non-negative number indicating how many values have been parsed and stored successfully,
 * equal to PROC_PARSER_SUCCESS if all values were parsed successfully
 * @return PROC_PARSER_TOTAL_USAGE_LINE if line started exactly with 'cpu ' and was skipped, without altering the result array.
 * @return PROC_PARSER_DISCARD_LINE if did not start with 'cpu' and was skipped, without altering the result array.
 * @return PROC_PARSER_FAIL if error occurred.
 *  
 * This distinction can be used for telling whether currently parsed section is before
 * the section starting with cpu0, cpu1,... or past it. 
 */
int proc_parser_parse_line(const char buffer[restrict static 5], uint64_t result[restrict static 10]);

/**
 * @brief use row retrieved from proc/stat to compute idle time and total time
 * 
 * @param core_line array containing parsed fields from core? line.
 * @return ProcParserCpuTime pair of values representing idle time and total time
 */
ProcParserCpuTime proc_parser_compute_core_time(const uint64_t core_line[static 10]);

/**
 * @brief Compute core usage in %.
 * 
 * @param previous previous time
 * @param current current time
 * @return double value between [0,1] representing % of average core usage.
 */
double proc_parser_cpu_time_compute_usage(const ProcParserCpuTime* previous, const ProcParserCpuTime* current);

#endif

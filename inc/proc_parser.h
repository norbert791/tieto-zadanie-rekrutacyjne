#ifndef PROC_PARSER_H
#define PROC_PARSER_H

#include <inttypes.h>

typedef enum proc_parser_result {
    TOTAL_USAGE_LINE,
    CORE_USAGE_LINE,
    DISCARD_LINE,
    FAIL,
} proc_parser_result;

/**
 * @brief Parser /proc/stat line contained in buffer and insert integers with
 * retrieved data into result array.
 * 
 * If buffer does not contain line starting with 'cpu?' result will not be altered
 * 
 * @param buffer null-terminated string with at least 4 characters
 * @param result array for storing result with space for at least 9 elements
 * @return 
 * 
 * Non-negative number indicating how many values have been parsed and stored successfuly
 * -1 if line started with 'cpu' and was skipped, without altering result array.
 * -2 if did not start with 'cpu' and was skipped, without altering result array.
 */
int proc_parser_parse_line(char buffer[static 5], uint64_t result[static 9]);


#endif
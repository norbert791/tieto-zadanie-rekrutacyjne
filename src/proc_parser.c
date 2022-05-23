#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "proc_parser.h"


int proc_parser_parse_line(char buffer[static 5], uint64_t result[static 9]) {

    static const char* specifiers = PRId64 PRId64 PRId64 PRId64 PRId64 PRId64 PRId64 PRId64 PRId64;
    static const int number_of_specifiers = 9;

    if (strncmp(buffer, "cpu", 3) != 0) {
        return -2;
    }
    else if (isdigit(buffer[3]) == 0) {
        return -1;
    }
    
    return sscanf(buffer, specifiers, result);
}

#include "proc_parser.h"
#include "assert.h"

static void parse_line_test() {

    const char cpu0[] = "123 123 0 213 123 1 48 9 10";
    uint64_t array[10];

    int t = proc_parser_parse_line(cpu0, array);
    assert(t == 9);
}


int main() {

    return 0;
}
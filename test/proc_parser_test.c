#include <tgmath.h>
#include "proc_parser.h"
#include "assert.h"

static void parse_line_test() {

    {
        const char cpu0[] = "cpu0 123 123 0 213 123 1 48 9 10";
        uint64_t array[10];

        int t = proc_parser_parse_line(cpu0, array);
        assert(t == 9);
    }

    {
        const char cpu0[] = "cpu0 123 123 0 213 123 1 48 9 10 15";
        uint64_t array[10];

        int t = proc_parser_parse_line(cpu0, array);
        assert(t == 10);
    }

    {
        const char cpu0[] = "cpu14 123 123 0 213 123 1 48 9 10 0";
        uint64_t array[10];

        proc_parser_parse_line(cpu0, array);
        
        assert(array[0] == 123);
        assert(array[1] == 123);
        assert(array[2] == 0);
        assert(array[3] == 213);
        assert(array[4] == 123);
        assert(array[5] == 1);
        assert(array[6] == 48);
        assert(array[7] == 9);
        assert(array[8] == 10);
        assert(array[9] == 0);
    }

    {
        const char cpu0[] = "cpu0 123 123 0 213 12";
        uint64_t array[10];

        int t = proc_parser_parse_line(cpu0, array);
        assert(t == 5);
    }

    {
        const char cpu[] = "cpu 123 123 0 213 123 1 48 9 10";
        uint64_t array[10];

        int t = proc_parser_parse_line(cpu, array);
        assert(t == -1);
    }

    {
        const char gibberish[] = "gibberish";
        uint64_t array[10];

        int t = proc_parser_parse_line(gibberish, array);
        assert(t == -2);
    }

}

static void compute_core_usage_test() {

    const uint64_t prev[10] = {89133, 407, 43245, 1141342, 36564, 5537, 819, 0, 0, 0};
    const uint64_t current[10] =  {107454, 493, 50403, 1351241, 39551, 6702, 986, 0, 0, 0};

    double expected_value = 0.1121722557479054 /*computed manually with bc*/;
    double real = proc_parser_compute_core_usage(prev, current);
    assert(fabs(real - expected_value) < 0.1);
}


int main() {

    compute_core_usage_test();
    parse_line_test();

    return 0;
}
#include <assert.h>
#include <inttypes.h>
#include "circular_buffer.h"

#define buffer_size 20

static void new_test() {
    Circular_Buffer* buffer = Circular_Buffer_new(0,10);
    assert(buffer == NULL);
    buffer = Circular_Buffer_new(5,0);
    assert(buffer == NULL);
    buffer = Circular_Buffer_new(buffer_size, sizeof(char));
    assert(buffer != NULL);
    free(buffer);
}

static void insert_test() {

    Circular_Buffer* buffer = Circular_Buffer_new(buffer_size, sizeof(char));
    char test[buffer_size + 1] = {0};

    for (size_t i = 0; i < buffer_size; i++) {
        int_fast16_t res = Circular_Buffer_insert_single(buffer, &test[i]);
        /*insertions should be successful*/
        assert(res == 1);
    }
    /*Maximum capacity reached*/
    assert(Circular_Buffer_insert_single(buffer, &test[buffer_size]) == 0);
    /*NULL insertion should not be possible*/
    assert(Circular_Buffer_insert_single(buffer, NULL) == -1);
    /*Insertion to NULL should not be possible*/
    assert(Circular_Buffer_insert_single(NULL, &test[0]) == -1);

    free(buffer);
}

static void remove_test() {
    Circular_Buffer* buffer = Circular_Buffer_new(buffer_size, sizeof(double));
    double test[buffer_size];
    for (size_t i = 0; i < buffer_size; i++) {
        test[i] = (double) i;
    }
    double temp = 0.0;
    /*Removing element from empty buffer should result with 0*/
    assert(Circular_Buffer_remove_single(buffer, &temp) == 0);
    /*Removing element from NULL should result in -1;*/
    assert(Circular_Buffer_remove_single(NULL, &temp) == -1);

    assert(Circular_Buffer_read_available(buffer) == 0);
    assert(Circular_Buffer_write_available(buffer) == buffer_size);

    for (size_t i = 0; i < buffer_size; i++) {
        /*Assertions about inserting are made in another function*/
        Circular_Buffer_insert_single(buffer, &test[i]);
    }
    
    assert(Circular_Buffer_read_available(buffer) == buffer_size);
    assert(Circular_Buffer_write_available(buffer) == 0);

    for (size_t i = 0; i < buffer_size; i++) {
        int_fast16_t res = Circular_Buffer_remove_single(buffer, &temp);
        assert(temp == test[i]);
        assert(res == 1);
    }

    /*The buffer should be empty again*/
    assert(Circular_Buffer_remove_single(buffer, &temp) == 0);
    assert(Circular_Buffer_write_available(buffer) == buffer_size);
    assert(Circular_Buffer_read_available(buffer) == 0);
    free(buffer);

    
}

/*Test scenario where we insert and remove elements in portions*/
static void insert_remove_test() {
    uint32_t test[buffer_size];
    uint32_t temp;
    for (size_t i = 0; i < buffer_size; i++) {
        test[i] = i;
    }

    Circular_Buffer* buffer = Circular_Buffer_new(buffer_size, sizeof(uint32_t));

    for (size_t i = 0; i < buffer_size / 2; i++) {
        Circular_Buffer_insert_single(buffer, &test[i]);
        Circular_Buffer_remove_single(buffer, &temp);
    }

    for (size_t i = 0; i < buffer_size; i++) {
        Circular_Buffer_insert_single(buffer, &test[i]);
    }

    for (size_t i = 0; i < buffer_size; i++) {
        Circular_Buffer_remove_single(buffer, &temp);
        assert(temp == test[i]);
    }

    free(buffer);
}

#undef buffer_size

int main() {

    insert_test();
    remove_test();
    new_test();
    insert_remove_test();

    return 0;
}
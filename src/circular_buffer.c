#include <stdlib.h>
#include <string.h>
#include "circular_buffer.h"

struct Circular_Buffer {
    size_t read_index;
    size_t write_index;
    size_t element_size;
    size_t buffer_max_size;
    size_t num_of_elements; 
    uint8_t buffer[]; /*FAM*/

};

Circular_Buffer* Circular_Buffer_new(size_t buffer_size, size_t element_size) {
    if (buffer_size == 0 || element_size == 0) {
        return NULL;
    }
    
    //Size of Circular_Buffer + size of FAM
    Circular_Buffer* result = calloc(1, sizeof(*result) + buffer_size * element_size);
    
    if (result == NULL) {
        return NULL;
    }

    result->buffer_max_size = buffer_size;
    result->element_size = element_size;

    return result;
}

void Circular_Buffer_delete(Circular_Buffer* restrict c_b) {
    if (c_b != NULL) {
        free(c_b);
    }
}

int_fast16_t Circular_Buffer_insert_single(Circular_Buffer* c_b, const void* restrict element) {
    if (c_b == NULL ||  element == NULL) {
        return -1;
    }
    else if (c_b->num_of_elements < c_b->buffer_max_size) {
            memcpy(&(c_b->buffer[c_b->write_index * c_b->element_size]), element, c_b->element_size);
            c_b->num_of_elements++;
            c_b->write_index++;
            c_b->write_index %= c_b->buffer_max_size;
            return 1;
    }
    return 0;
}

int_fast16_t Circular_Buffer_remove_single(Circular_Buffer* c_b, void* restrict dest) {
    if (c_b == NULL  || dest == NULL) {
        return -1;
    }
    else if (c_b->num_of_elements == 0) {
        return 0;
    }
    else {
        memcpy(dest, &(c_b->buffer[c_b->read_index *  c_b->element_size]), c_b->element_size);
        c_b->num_of_elements--;
        c_b->read_index++;
        c_b->read_index %= c_b->buffer_max_size;
        return 1;
    }
}

size_t Circular_Buffer_read_available(const Circular_Buffer* const restrict c_b) {
    return c_b->num_of_elements;
}

size_t Circular_Buffer_write_available(const Circular_Buffer* const restrict c_b) {
    return c_b->buffer_max_size - c_b->num_of_elements;
}
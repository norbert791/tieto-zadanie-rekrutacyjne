#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <pthread.h>

enum Circular_Buffer_Error {
    NULL_PTR_ERROR = -1,
};

typedef struct Circular_Buffer Circular_Buffer;

Circular_Buffer* circular_buffer_new(size_t buffer_size, size_t element_size);

void circular_buffer_delete(Circular_Buffer* restrict buffer);

int circular_buffer_insert_single(Circular_Buffer* buffer, const void* restrict element);

int circular_buffer_remove_single(Circular_Buffer* buffer, void* restrict dest);

size_t circular_buffer_write_available(const Circular_Buffer* restrict c_b);

size_t circular_buffer_read_available(const Circular_Buffer* restrict c_b);

size_t circular_buffer_insert_multi(const Circular_Buffer* restrict c_b, size_t number_of_elements, const void* restrict array);

size_t circular_buffer_remove_multi(const Circular_Buffer* restrict c_b, size_t number_of_elements, const void* restrict array);

size_t circular_buffer_File_Write(FILE* file, const Circular_Buffer* restrict c_b, size_t number_of_bytes);

#endif
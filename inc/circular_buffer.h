#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>

typedef struct Circular_Buffer Circular_Buffer;

Circular_Buffer* Circular_Buffer_new(size_t buffer_size, size_t element_size);

void Circular_Buffer_delete(Circular_Buffer* restrict buffer);

int_fast16_t Circular_Buffer_insert_single(Circular_Buffer* buffer, const void* restrict element);

int_fast16_t Circular_Buffer_remove_single(Circular_Buffer* buffer, void* restrict dest);

size_t Circular_Buffer_write_available(const Circular_Buffer* restrict c_b);

size_t Circular_Buffer_read_available(const Circular_Buffer* restrict c_b);

size_t Circular_Buffer_insert_multi(const Circular_Buffer* restrict c_b, size_t number_of_elements, const void* restrict array);

size_t Circular_Buffer_remove_multi(const Circular_Buffer* restrict c_b, size_t number_of_elements, const void* restrict array);

size_t Circular_Buffer_File_Write(FILE* file, const Circular_Buffer* restrict c_b, size_t number_of_bytes);

#endif
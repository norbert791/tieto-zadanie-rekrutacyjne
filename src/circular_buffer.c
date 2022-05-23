#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "circular_buffer.h"

struct Circular_Buffer {
    size_t read_index;
    size_t write_index;
    size_t element_size;
    size_t buffer_max_size;
    size_t num_of_elements;
    
    uint8_t buffer[]; /*FAM*/
};

Circular_Buffer* circular_buffer_new(size_t buffer_size, size_t element_size) {
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

void circular_buffer_delete(Circular_Buffer* restrict buffer) {
    if (buffer != NULL) {
        free(buffer);
    }
}

int circular_buffer_insert_single(Circular_Buffer* buffer, const void* restrict element) {
    if (buffer == NULL || element == NULL) {
        return -1;
    }
    else if (buffer->num_of_elements < buffer->buffer_max_size) {
            memcpy(&(buffer->buffer[buffer->write_index * buffer->element_size]), element, buffer->element_size);
            buffer->num_of_elements++;
            buffer->write_index++;
        buffer->write_index %= buffer->buffer_max_size;
            return 1;
    }
    return 0;
}

int circular_buffer_remove_single(Circular_Buffer* buffer, void* restrict dest) {
    if (buffer == NULL || dest == NULL) {
        return -1;
    }
    else if (buffer->num_of_elements == 0) {
        return 0;
    }
    else {
        memcpy(dest, &(buffer->buffer[buffer->read_index * buffer->element_size]), buffer->element_size);
        buffer->num_of_elements--;
        buffer->read_index++;
        buffer->read_index %= buffer->buffer_max_size;
        return 1;
    }
}

size_t circular_buffer_read_available(const Circular_Buffer  restrict *c_b) {
    return c_b->num_of_elements;
}

size_t circular_buffer_write_available(const Circular_Buffer  restrict *c_b) {
    return c_b->buffer_max_size - c_b->num_of_elements;
}


int Synchronized_Circular_Buffer_initialize(Synchronized_Circular_Buffer* wrapper, Circular_Buffer* buffer) {
    if (wrapper == NULL) {
        return -1;
    }
    wrapper->buffer = buffer;
    wrapper->mutex = PTHREAD_MUTEX_INITIALIZER;
    wrapper->consumer = PTHREAD_COND_INITIALIZER;
    wrapper->producer = PTHREAD_COND_INITIALIZER;

    return 0;
}

void Synchronized_Circular_Buffer_destroy(Synchronized_Circular_Buffer* wrapper) {

}

int Synchronized_Circular_Buffer_lock(Synchronized_Circular_Buffer* wrapepr) {

}

int Synchronized_Circular_Buffer_unlock(Synchronized_Circular_Buffer* wrapepr) {

}

int Synchronized_Circular_Buffer_notify_reader(Synchronized_Circular_Buffer* wrapepr) {

}

int Synchronized_Circular_Buffer_notify_writer(Synchronized_Circular_Buffer* wrapepr) {

}

/**
 * @file circular_buffer.h
 * @brief Functional API for Circular buffers (FIFO queues).
 * 
 */

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <pthread.h>

enum Circular_Buffer_Error {
    NULL_PTR_ERROR = -1,
};

/**
 * @brief Circular buffer structure.
 * 
 */
typedef struct Circular_Buffer Circular_Buffer;

/**
 * @brief Allocates new Circular_Buffer of given size.
 * 
 * @param buffer_size number of elements that will fit into buffer
 * @param element_size Size of single element
 * @return Circular_Buffer* Pointer to the allocated buffer on success. NULL if at least one of the arguments was equal to 0 or allocation failed 
 */
Circular_Buffer* circular_buffer_new(size_t buffer_size, size_t element_size);

/**
 * @brief Deletes allocated Circular_Buffer.
 * 
 * @param buffer Pointer to either valid Circular_Buffer or nullptr. 
 * In former case, the pointed Circular_Buffer will be deallocated. 
 * In lattter one, nothing will hapen
 */
void circular_buffer_delete(Circular_Buffer* restrict buffer);

/**
 * @brief Insert single element into circular buffer.
 * 
 * @param buffer pointer to valid Circular_Buffer which will store the inserted element
 * @param element pointer to th eelement that shall be copied into the buffer. If the size of inserted element
 * is different element_size of allocated buffer, the behaviour is undefined.
 * @return int 1 iff insertion was successful, 0 iff buffer is already full and element could not be inserted. 
 * -1 if at least one of the argumets was nullptr
 */
int circular_buffer_insert_single(Circular_Buffer* restrict buffer, const void* restrict element);

/**
 * @brief Retrieve single element from circular buffer to memory pointed by dest, then remove the element.
 * 
 * 
 * @param buffer pointer to valid Circular_Buffer from which an element will be retrieved.
 * @param dest pointer to memory where retrieved object will be stored.
 * @return 1 iff element was succesfully retrieved. 0 iff buffer was empty. -1 if at least one of the parameter was nullptr.
 */
int circular_buffer_remove_single(Circular_Buffer* restrict buffer, void* restrict dest);

/**
 * @brief Return number of elements that is aveilable for write
 * 
 * @param c_b pointer to valid Circular_Buffer
 * @return size_t number of elements aveilable for retrieval
 */
size_t circular_buffer_write_available(const Circular_Buffer* restrict c_b);

/**
 * @brief Return number of lements that is available for read
 * 
 * @param c_b pointer to valid Circular_Buffer
 * @return size_t number of elements available for read
 */
size_t circular_buffer_read_available(const Circular_Buffer* restrict c_b);


#endif
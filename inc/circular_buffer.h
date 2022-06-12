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

enum ECircularBufferError {
    NULL_PTR_ERROR = -1,
};

/**
 * @brief Circular buffer structure.
 * 
 */
typedef struct CircularBuffer CircularBuffer;

/**
 * @brief Allocates new CircularBuffer of given size.
 * 
 * @param buffer_size number of elements that will fit into the buffer
 * @param element_size Size of single element (in bytes)
 * @return CircularBuffer* Pointer to the allocated buffer on success. NULL if at least one of the arguments was equal to 0 or allocation failed 
 */
CircularBuffer* circular_buffer_new(size_t buffer_size, size_t element_size);

/**
 * @brief Deletes allocated CircularBuffer.
 * 
 * @param buffer Pointer to either valid CircularBuffer or nullptr. 
 * In former case, the pointed CircularBuffer will be deallocated. 
 * In lattter one, nothing will hapen
 */
void circular_buffer_delete(CircularBuffer* buffer);

/**
 * @brief Insert single element into circular buffer.
 * 
 * @param buffer pointer to valid CircularBuffer which will store the inserted element
 * @param element pointer to the eelement that shall be copied into the buffer. If the size of the inserted element
 * is not equal to element_size of the buffer, the behaviour is undefined.
 * @return 1 iff insertion was successful, 0 iff buffer is already full and element could not be inserted. 
 * -1 if at least one of the argumets was nullptr
 */
int circular_buffer_insert_single(CircularBuffer* restrict buffer, const void* restrict element);

/**
 * @brief Retrieve single element from circular buffer to memory pointed by dest, then remove the element.
 * 
 * 
 * @param buffer pointer to valid CircularBuffer from which an element will be retrieved.
 * @param dest pointer to memory where retrieved object will be stored.
 * @return 1 iff element was succesfully retrieved. 0 iff buffer was empty. NULL_PTR_ERROR if at least one of the parameter was nullptr.
 */
int circular_buffer_remove_single(CircularBuffer* restrict buffer, void* restrict dest);

/**
 * @brief Return number of elements available for write
 * 
 * @param c_b pointer to valid CircularBuffer
 * @return size_t number of elements that can be sucessfuly wrote into the buffer
 */
size_t circular_buffer_write_available(const CircularBuffer* c_b);

/**
 * @brief Return number of elements available for read
 * 
 * @param c_b pointer to valid CircularBuffer
 * @return size_t number of elements available for read
 */
size_t circular_buffer_read_available(const CircularBuffer* c_b);


#endif
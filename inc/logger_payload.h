/**
 * @file logger_payload.h
 * @brief interface used for sending logs to thread_logger
 * 
 */

#ifndef LOGGER_PAYLOAD_H
#define LOGGER_PAYLOAD_H

typedef struct Logger_Payload Logger_Payload;

typedef enum logger_payload_type {
    LOGGER_PAYLOAD_TYPE_WARNING = 0,
    LOGGER_PAYLOAD_TYPE_ERROR = 1,
} logger_payload_type;

/**
 * @brief Allocate new payload of selected type
 * 
 * @param type type of payload
 * @param message pointer to valid string of byte size at least 1 (empty string)
 * @return pointer to newly allocated payload on success, NULL on failure or if message is an empty string
 */
Logger_Payload* logger_payload_new(logger_payload_type type, const char message[restrict static 1]);

/**
 * @brief Free memorty occupied by payload
 * 
 * @param payload pointer to payload that is to be deleted
 */
void logger_payload_delete(Logger_Payload* payload);

/**
 * @brief Get pointer to string containing message.
 * 
 * @param payload pointer to payload whose message shall be recieved
 * @return const char* pointer to read-only string containing message.
 */
const char* logger_payload_get_message(Logger_Payload* payload);

/**
 * @brief get type of payload
 * 
 * @param payload pointer to payload whose type shall be retrieved
 * @return logger_payload_type
 */
logger_payload_type logger_payload_get_type(Logger_Payload* payload);

/**
 * @brief get pointer to read-only string representing logger_payload_constant
 * 
 * @param type payload type
 * @return const char* pointer to read-only sring representing payload type
 */
const char* logger_payload_type_to_str(logger_payload_type type);

#endif
/**
 * @file logger_payload.h
 * @brief interface used for sending logs to thread_logger
 * 
 */

#ifndef LOGGER_PAYLOAD_H
#define LOGGER_PAYLOAD_H

typedef struct LoggerPayload LoggerPayload;

typedef enum ELoggerPayloadType {
    LOGGER_PAYLOAD_TYPE_WARNING = 0,
    LOGGER_PAYLOAD_TYPE_ERROR = 1,
} ELoggerPayloadType;

/**
 * @brief Allocate new payload of selected type
 * 
 * @param type type of payload
 * @param message pointer to valid string of byte size at least 1 (empty string)
 * @return pointer to newly allocated payload on success, NULL on failure or if message is an empty string
 */
LoggerPayload* logger_payload_new(ELoggerPayloadType type, const char message[restrict static 1]);

/**
 * @brief Free memory occupied by payload
 * 
 * @param payload pointer to payload that is to be deleted
 */
void logger_payload_delete(LoggerPayload* payload);

/**
 * @brief Get pointer to string containing message.
 * 
 * @param payload pointer to payload whose message shall be recieved
 * @return const char* pointer to read-only string containing payload message.
 */
const char* logger_payload_get_message(LoggerPayload* payload);

/**
 * @brief get the type of payload
 * 
 * @param payload pointer to payload whose type shall be retrieved
 * @return ELoggerPayloadType
 */
ELoggerPayloadType logger_payload_get_type(LoggerPayload* payload);

/**
 * @brief get the pointer to read-only string representing logger_payload_constant
 * 
 * @param type payload type
 * @return const char* pointer to read-only sring representing payload type
 */
const char* logger_payload_type_to_str(ELoggerPayloadType type);

#endif
#ifndef LOGGER_PAYLOAD_H
#define LOGGER_PAYLOAD_H


typedef struct Logger_Payload Logger_Payload;

typedef enum logger_payload_type {
    LOGGER_PAYLOAD_TYPE_WARNING = 0,
    LOGGER_PAYLOAD_TYPE_ERROR = 1,
} logger_payload_type;

Logger_Payload* logger_payload_new(logger_payload_type type, const char message[restrict static 1]);

void logger_payload_delete(Logger_Payload* restrict payload);

const char* logger_payload_get_message(Logger_Payload* restrict payload);

logger_payload_type logger_payload_get_type(Logger_Payload* restrict payload);

const char* logger_payload_type_to_str(logger_payload_type type);

#endif
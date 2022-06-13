#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "logger_payload.h"

typedef struct LoggerPayload {
    size_t message_size;
    ELoggerPayloadType type;
    char message[]; /*FAM*/
} LoggerPayload;

LoggerPayload* logger_payload_new(const ELoggerPayloadType type, const char message[const restrict static 1]) {
    if (strcmp(message, "") == 0) {
        return NULL;
    }

    const size_t message_size = strlen(message);

    LoggerPayload* result = malloc(sizeof(*result) + sizeof(*result->message) * (message_size + 1));
    if (result == NULL) {
        errno = 0;
        return NULL;
    }

    result->message_size = message_size;
    result->type = type;
    strcpy(result->message, message);   

    return result;
}

void logger_payload_delete(LoggerPayload* const payload) {
    free(payload);
}

const char* logger_payload_get_message(LoggerPayload* const payload) {
    return payload->message;
}

ELoggerPayloadType logger_payload_get_type(LoggerPayload* const payload) {
    return payload->type;
}

const char* logger_payload_type_to_str(ELoggerPayloadType type) {
    static const char* message_str[] = {"Warning", "Error"};
    return message_str[type];
}

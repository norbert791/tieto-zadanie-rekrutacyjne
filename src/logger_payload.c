#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "logger_payload.h"

typedef struct Logger_Payload {
    size_t message_size;
    logger_payload_type type;
    char message[]; //FAM
} Logger_Payload;

Logger_Payload* logger_payload_new(const logger_payload_type type, const char message[const restrict static 1]) {

    if (strcmp(message, "") == 0) {
        return NULL;
    }

    const size_t message_size = strlen(message);

    Logger_Payload* result = malloc(sizeof(*result) + sizeof(*result->message) * (message_size + 1));
    if (result == NULL) {
        errno = 0;
        return NULL;
    }

    result->message_size = message_size;
    result->type = type;
    strcpy(result->message, message);   

    return result;
}

void logger_payload_delete(Logger_Payload* restrict const payload) {
    free(payload);
}

const char* logger_payload_get_message(Logger_Payload* restrict const payload) {
    return payload->message;
}

logger_payload_type logger_payload_get_type(Logger_Payload* restrict const payload) {
    return payload->type;
}

const char* logger_payload_type_to_str(logger_payload_type type) {
    static const char* message_str[] = {"Warning", "Error"};
    return message_str[type];
}
#include <assert.h>
#include <string.h>
#include "logger_payload.h"

static void logger_new_delete_test();
static void logger_get_message_test();
static void logger_get_type_test();
static void logger_type_to_string_test();

static void logger_new_delete_test() {
    {
        LoggerPayload* payload = logger_payload_new(LOGGER_PAYLOAD_TYPE_WARNING, "TEST");
        logger_payload_delete(payload);
    }

    {
        LoggerPayload* payload = logger_payload_new(LOGGER_PAYLOAD_TYPE_ERROR, "");
        assert(payload == NULL);
    }

}

static void logger_get_message_test() {
    LoggerPayload* payload = logger_payload_new(LOGGER_PAYLOAD_TYPE_WARNING, "TEST");
    assert(strcmp(logger_payload_get_message(payload), "TEST") == 0);
    logger_payload_delete(payload);
}

static void logger_get_type_test() {
    LoggerPayload* payload = logger_payload_new(LOGGER_PAYLOAD_TYPE_ERROR, "TEST");
    assert(logger_payload_get_type(payload) == LOGGER_PAYLOAD_TYPE_ERROR);
    logger_payload_delete(payload);
}

/*This test shall succed if the program doesn't crash*/
static void logger_type_to_string_test() {
    logger_payload_type_to_str(LOGGER_PAYLOAD_TYPE_ERROR);
    logger_payload_type_to_str(LOGGER_PAYLOAD_TYPE_ERROR);
}

int main() {
    logger_new_delete_test();
    logger_get_message_test();
    logger_get_type_test();
    logger_type_to_string_test();
    return 0;
}
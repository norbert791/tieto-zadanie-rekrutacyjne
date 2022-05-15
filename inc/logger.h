#ifndef LOGGER_H
#define LOGGER_H

typedef struct  Logger Logger;

Logger* Logger_new();
Logger* Logger_delete();
Logger* Logger_persist_message(char message[restrict static 1]);

#endif
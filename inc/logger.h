#ifndef LOGGER_H
#define LOGGER_H

typedef struct  Logger Logger;

Logger* Logger_new();
void Logger_delete(Logger*);
Logger* Logger_persist_message(Logger* logger, const char message[restrict static 1]);

#endif
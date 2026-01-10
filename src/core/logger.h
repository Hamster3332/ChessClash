#ifndef LOGGER_H
#define LOGGER_H
#include <string>

void deprecationWarn(std::string func);

void logInitiate();

void logEnter(std::string func);

void logExit(std::string Reason);

void logInfo(std::string Info);

void logClose();

#endif

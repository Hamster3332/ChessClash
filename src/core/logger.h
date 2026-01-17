#ifndef LOGGER_H
#define LOGGER_H
#include <string>

void deprecationWarn(std::string func);

void logInitiate();

void logID(int id);

void logEnter(std::string func, int id);

void logExit(std::string Reason, int id);

void logInfo(std::string Info, int id);

void logEnter(std::string func);

void logExit(std::string Reason);

void logInfo(std::string Info);

void logClose();

#endif

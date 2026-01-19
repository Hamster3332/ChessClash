#include "logger.h"
#include <fstream>
#include <iostream>

std::ofstream LOGGER_file;
int LOGGER_currentID = 0;
bool first = true;

void deprecationWarn(std::string func) {
    std::cout << "Function Deprecated: " << func << std::endl;
}

void logInitiate() {
    LOGGER_file.open("Logs.json");
    LOGGER_file << "[";
    bool first = true;
}

void logID(int id) {
    LOGGER_currentID = id;
}

void logEnter(std::string func, int id) {
    if (LOGGER_currentID == id){
        if (first) LOGGER_file << "\n{\"" << func << "\": [";
        else LOGGER_file << ",\n{\"" << func << "\": [";
        first = true;
    }
}

void logEnter(std::string func) {
    if (LOGGER_currentID == 0){
        if (first) LOGGER_file << "\n{\"" << func << "\": [";
        else LOGGER_file << ",\n{\"" << func << "\": [";
        first = true;
    }
}

void logExit(std::string Reason, int id){
    if (LOGGER_currentID == id){
        if (first) LOGGER_file << " \"ExitReason" << Reason << "\"]}";
        else LOGGER_file << ", \"ExitReason" << Reason << "\"]}";
        first = false;
    }
}

void logExit(std::string Reason){
    if (LOGGER_currentID == 0){
        if (first) LOGGER_file << " \"ExitReason" << Reason << "\"]}";
        else LOGGER_file << ", \"ExitReason" << Reason << "\"]}";
        first = false;
    }
}

void logInfo(std::string Info, int id){
    if (LOGGER_currentID == id){
        if (first) LOGGER_file << " \"Info: " << Info << "\"";
        else LOGGER_file << ", \"Info: " << Info << "\"";
        first = false;
    }
}

void logInfo(std::string Info){
    if (LOGGER_currentID == 0){
        if (first) LOGGER_file << " \"Info: " << Info << "\"";
        else LOGGER_file << ", \"Info: " << Info << "\"";
        first = false;
    }
}

void logClose(){
    LOGGER_file << "\n]";
    LOGGER_file.close();
}

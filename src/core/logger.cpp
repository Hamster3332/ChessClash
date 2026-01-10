#include "logger.h"
#include <fstream>
#include <iostream>

std::ofstream file;

void deprecationWarn(std::string func) {
    std::cout << "Function Deprecated: " << func << std::endl;
}

void logInitiate() {
    file.open("Logs.json");
    file << "[0";
}

void logEnter(std::string func){
    file << ",\n{\"" << func << "\": [ 0 ";
}

void logExit(std::string Reason){
    file << ",\"ExitReason: " << Reason <<"\"]} ";
}

void logInfo(std::string Info){
    file << ",\"Info: " << Info << "\"";
}

void logClose(){
    file << "]";
    file.close();
}

#include "logger.h"
#include <fstream>
#include <iostream>

std::ofstream file;

void deprecationWarn(std::string func) {
    std::cout << "Function Deprecated: " << func << std::endl;
}

void initiate() {
    file.open("Loggs.json");
    file << "{}";
    file.close();
}

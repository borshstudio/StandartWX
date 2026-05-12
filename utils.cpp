#include "utils.h"

#include <iostream>
#include <cstdlib>



void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}



void printError(const std::string& msg) {
    std::cout << "\033[1;31m" << msg << "\033[0m\n";   // красный
}

void printOk(const std::string& msg) {
    std::cout << "\033[1;32m" << msg << "\033[0m\n";   // зелёный
}

void printInfo(const std::string& msg) {
    std::cout << "\033[1;36m" << msg << "\033[0m\n";   // голубой
}

void printWarning(const std::string& msg) {
    std::cout << "\033[1;33m" << msg << "\033[0m\n";   // жёлтый
}
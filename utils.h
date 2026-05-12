#ifndef UTILS_H
#define UTILS_H

#include <string>


void clearScreen();

void printError(const std::string& msg);
void printOk(const std::string& msg);
void printInfo(const std::string& msg);
void printWarning(const std::string& msg);

#endif
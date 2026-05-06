#include "api.h"
#include "weather.h"

#include <iostream>
#include <fstream>
#include <string>


std::string loadApiKey() {
    std::ifstream file("api_key.txt");
    std::string apiKey;
    if (file.is_open()) {
        std::getline(file, apiKey);
    }
    return apiKey;
}

void saveApiKey(const std::string& apiKey) {
    std::ofstream file("api_key.txt");
    file << apiKey;
}

std::string getValidApiKey() {
    std::string apiKey = loadApiKey();

    if (!apiKey.empty()) {
        std::cout << "[INFO] Найден сохраненный API-ключ. Проверяем...\n";
        if (checkApiKey(apiKey)) {
            std::cout << "[OK] API-ключ работает.\n";
            return apiKey;
        }
        std::cout << "[ERROR] Сохраненный API-ключ не работает.\n";
    }

    while (true) {
        std::cout << "\nВведите API-ключ OpenWeatherMap (0 — выход): ";
        std::getline(std::cin, apiKey);

        if (apiKey == "0") {
            return "";
        }

        if (apiKey.empty()) {
            std::cout << "[ERROR] API-ключ не может быть пустым.\n";
            continue;
        }

        std::cout << "[INFO] Проверяем API-ключ...\n";

        if (checkApiKey(apiKey)) {
            saveApiKey(apiKey);
            std::cout << "[OK] API-ключ сохранен.\n";
            return apiKey;
        }

        std::cout << "[ERROR] API-ключ неправильный.\n";
    }
}



int main() {

    std::cout << "Добро пожаловать в приложение прогноза погоды!\n";

    std::string apiKey = getValidApiKey();
    if (apiKey.empty()) {
        std::cout << "Выход из приложения.\n";
        return 0;
    }

    return 0;
}

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

int main() {
    int choice;

    std::cout << "=== СтандартWX - Прогноз погоды ===" << std::endl;

    do {

        std::cout << "\nВыберите опцию:" << std::endl;
        std::cout << "1. Текущая погода" << std::endl;
        std::cout << "2. Прогноз погоды на 3 дня" << std::endl;
        std::cout << "3. Выход" << std::endl;
        std::cout << "Ваш выбор: ";

        if (!(std::cin >> choice)) {
            std::cout << "\nОшибка ввода. Пожалуйста, введите число." << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            choice = 0;
        }

        switch(choice) {
            case 1:
                std::cout << "\n" << getCurrentWeather() << std::endl;
                break;
            case 2:
                std::cout << "\n" << getForecastWeather() << std::endl;
                break;
            case 3:
                std::cout << "\nДо свидания!" << std::endl;
                break;
            default:
                std::cout << "\nНеверный выбор. Попробуйте снова." << std::endl;
                break;
        }

    } while(choice != 3);

    return 0;
}

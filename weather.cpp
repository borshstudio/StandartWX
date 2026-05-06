#include "weather.h"
#include "api.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>



void showWeatherForCity(const std::string& city, const std::string& apiKey) {
    ApiResult result = requestWeatherFromApi(city, apiKey);

    if (!result.success) {
        std::cout << "[ERROR] " << result.errorMessage << "\n";
        return;
    }

    WeatherInfo w = result.weather;

    std::cout << "\n==============================\n";
    std::cout << "Сегодня в городе: " << w.city << "\n";
    std::cout << "==============================\n";
    std::cout << "[T] Температура:     " << w.temperature << " °C\n";
    std::cout << "[~] Ощущается как:  " << w.feelsLike << " °C\n";
    std::cout << "[W] Погода:          " << w.description << "\n";
    std::cout << "[H] Влажность:       " << w.humidity << " %\n";
    std::cout << "[V] Ветер:           " << w.windSpeed << " м/с\n";
    std::cout << "[P] Давление:        " << w.pressure << " гПа\n";
    std::cout << "==============================\n";
}



void showWeatherByCity(const std::string& apiKey) {
    while (true) {
        std::string city;
        std::cout << "\nВведите название города (латиницей, 0 — назад): ";
        std::getline(std::cin, city);

        if (city == "0") return;
        if (city.empty()) {
            std::cout << "[ERROR] Название города не может быть пустым.\n";
            continue;
        }

        ApiResult result = requestWeatherFromApi(city, apiKey);
        if (!result.success) {
            std::cout << "[ERROR] " << result.errorMessage << "\n";
            continue;
        }

        showWeatherForCity(city, apiKey);
        return;
    }
}



std::vector<std::string> loadFavorites() {
    std::vector<std::string> favorites;
    std::ifstream file("favorites.txt");
    std::string city;

    while (std::getline(file, city)) {
        if (!city.empty()) favorites.push_back(city);
    }
    return favorites;
}



void saveFavorites(const std::vector<std::string>& favorites) {
    std::ofstream file("favorites.txt");
    for (const auto& city : favorites) {
        file << city << "\n";
    }
}



void favoritesMenu(const std::string& apiKey) {
    while (true) {
        std::cout << "\n=== ИЗБРАННЫЕ ГОРОДА ===\n";
        std::cout << "1) Показать список\n";
        std::cout << "2) Добавить город\n";
        std::cout << "3) Удалить город\n";
        std::cout << "4) Показать погоду\n";
        std::cout << "0) Назад\n";
        std::cout << "Ваш выбор: ";

        std::string choice;
        std::getline(std::cin, choice);
        auto favorites = loadFavorites();
        [06.05.2026 15:24] ? ? ? ? s ? ? ? ? ? ? ? ? : if (choice == "1") {
            if (favorites.empty()) {
                std::cout << "Список пуст.\n";
            }
            else {
                for (int i = 0; i < favorites.size(); ++i)
                    std::cout << i + 1 << ") " << favorites[i] << "\n";
            }
        }
        else if (choice == "2") {
            std::string city;
            std::cout << "Введите город: ";
            std::getline(std::cin, city);
            if (!city.empty()) {
                favorites.push_back(city);
                saveFavorites(favorites);
                std::cout << "[OK] Город добавлен.\n";
            }
        }
        else if (choice == "3") {
            if (favorites.empty()) {
                std::cout << "Список пуст.\n";
                continue;
            }
            std::cout << "Введите номер: ";
            std::string n;
            std::getline(std::cin, n);
            int idx = std::stoi(n) - 1;
            if (idx >= 0 && idx < favorites.size()) {
                favorites.erase(favorites.begin() + idx);
                saveFavorites(favorites);
                std::cout << "[OK] Город удален.\n";
            }
        }
        else if (choice == "4") {
            if (favorites.empty()) continue;
            std::cout << "Введите номер: ";
            std::string n;
            std::getline(std::cin, n);
            int idx = std::stoi(n) - 1;
            if (idx >= 0 && idx < favorites.size()) {
                showWeatherForCity(favorites[idx], apiKey);
            }
        }
        else if (choice == "0") {
            return;
        }
    }
}



void aboutApp() {
    std::cout << "\nО приложении:\n";
    std::cout << "Консольное приложение прогноза погоды.\n";
    std::cout << "Используется сервис OpenWeatherMap.\n\n";
    std::cout << "Авторы приложения:\n - Левашов Дмитрий\n - Новиков Юрий \n - Грачев Александр\n\n";
}

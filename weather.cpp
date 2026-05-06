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
    // ToDo
}



void saveFavorites(const std::vector<std::string>& favorites) {
    // ToDo
}



void favoritesMenu(const std::string& apiKey) {
    // ToDo
}



void aboutApp() {
    std::cout << "\nО приложении:\n";
    std::cout << "Консольное приложение прогноза погоды.\n";
    std::cout << "Используется сервис OpenWeatherMap.\n\n";
    std::cout << "Авторы приложения:\n - Левашов Дмитрий\n - Новиков Юрий \n - Грачев Александр\n\n";
}

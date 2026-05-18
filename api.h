#ifndef API_H
#define API_H

#include <string>

// структура погоды
struct WeatherInfo {
    std::string city;
    double temperature;
    double feelsLike;
    int humidity;
    int pressure;
    double windSpeed;
    std::string description;
};

struct ApiResult {
    bool success;
    int statusCode;
    std::string errorMessage;
    WeatherInfo weather;
};

// ф/и для работы с API
ApiResult requestWeatherFromApi(const std::string& city, const std::string& apiKey);
bool checkApiKey(const std::string& apiKey);

#endif
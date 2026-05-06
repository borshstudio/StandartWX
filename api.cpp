
#include "api.h"

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

ApiResult requestWeatherFromApi(const std::string& city, const std::string& apiKey) {
    ApiResult result;

    cpr::Response response = cpr::Get(
        cpr::Url{"https://api.openweathermap.org/data/2.5/weather"},
        cpr::Parameters{
            {"q", city},
            {"appid", apiKey},
            {"units", "metric"},
            {"lang", "ru"}
        },
        cpr::Timeout{10000}
    );

    result.statusCode = response.status_code;

    if (response.error.code != cpr::ErrorCode::OK) {
        result.success = false;
        result.errorMessage = "Ошибка соединения. Проверьте интернет.";
        return result;
    }

    if (response.status_code == 401) {
        result.success = false;
        result.errorMessage = "API-ключ неправильный.";
        return result;
    }

    if (response.status_code == 404) {
        result.success = false;
        result.errorMessage = "Город не найден.";
        return result;
    }

    if (response.status_code != 200) {
        result.success = false;
        result.errorMessage =
            "Ошибка сервера. Код ошибки: " + std::to_string(response.status_code);
        return result;
    }

    try {
        json data = json::parse(response.text);

        result.weather.city = data.value("name", city);
        result.weather.temperature = data["main"].value("temp", 0.0);
        result.weather.feelsLike = data["main"].value("feels_like", 0.0);
        result.weather.humidity = data["main"].value("humidity", 0);
        result.weather.pressure = data["main"].value("pressure", 0);
        result.weather.windSpeed = data["wind"].value("speed", 0.0);

        if (data.contains("weather") && !data["weather"].empty()) {
            result.weather.description =
                data["weather"][0].value("description", "нет описания");
        } else {
            result.weather.description = "нет описания";
        }

        result.success = true;
        return result;
    }
    catch (...) {
        result.success = false;
        result.errorMessage = "Ошибка обработки данных от сервера.";
        return result;
    }
}

bool checkApiKey(const std::string& apiKey) {
    return requestWeatherFromApi("London", apiKey).success;
}

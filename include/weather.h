//
// Created by ASUS on 24.03.2026.
//

#ifndef _WX_WEATHER_H
#define _WX_WEATHER_H



#include <string>
#include <optional>


// константы
constexpr double KELVIN_OFFSET = 273.15;
constexpr double PASCAL_TO_MMHG = 0.75006;


// константы дл€ определни€ точности форматированного вывода
typedef enum { HUNDREDTHS = 3, INTEGER = 1, TENTHS = 2 } Precision;


// ѕ» с утилитами перевода и форматированного вывода
namespace WeatherUtils {
    double kelvin_To_Celsius(double kelvin);
    double pascal_To_MmHg(double pascal);
    double formatValue(double value, Precision precision);
    std::string formatTimestamp(long long timestamp, int timezoneOffset);
}


// класс погода_сейчас
class Weather_Now {
private:
    std::string cityName;                                   // название города
    std::optional<double> temperatureKelvin;                // температура в кельвинах
    std::optional<double> temperatureCelsius;               // температура в градусах цельси€
    std::optional<int> humidity;                            // влажность
    std::optional<double> pressurePascal;                   // давление в паскал€х
    std::optional<double> pressureMmHg;                     // давление в мм
    std::optional<double> windSpeed;                        // скорость ветра
    std::optional<int> windDegrees;                         // направление ветра
    std::optional<double> windGust;                         // порывы ветра
    std::optional<int> timezoneOffset;                      // часовой по€с
    std::optional<long long> measurementTime;               // врем€ замера
    std::optional<long long> sunsetTime, sunriseTime;       //врем€ заката и рассвета
    std::string description;                                // описание

public:
    explicit Weather_Now(const std::string& name);
    Weather_Now(
        const std::string& name,
        std::optional<double> kelvin = std::nullopt,
        std::optional<int> hum = std::nullopt,
        std::optional<double> pascal = std::nullopt,
        std::optional<double> windSpd = std::nullopt,
        std::optional<int> windDeg = std::nullopt,
        std::optional<double> gust = std::nullopt,
        std::optional<int> tzOffset = std::nullopt,
        std::optional<long long> measTime = std::nullopt,
        std::optional<long long> sunset = std::nullopt,
        std::optional<long long> sunrise = std::nullopt,
        const std::string& desc = "Ќет данных.");

    // геттеры
    std::string get_cityName() const;
    std::optional<double> get_temperatureKelvin(Precision precision = TENTHS) const;
    std::optional<double> get_temperatureCelsius(Precision precision = TENTHS) const;
    std::optional<int> get_humidity() const;
    std::optional<double> get_windSpeed(Precision precision = TENTHS) const;
    std::string get_description() const;
    std::string get_windDirectionText() const;
    std::optional<double> get_pressurePascal(Precision precision = INTEGER) const;
    std::optional<double> get_pressureMmHg(Precision precision = TENTHS) const;
    std::optional<double> get_windGust(Precision precision = TENTHS) const;
    std::string get_measurementTime() const;
    std::string get_sunriseTime() const;
    std::string get_sunsetTime() const;
    std::optional<int> get_timezoneOffset() const;

    // проверка дн€/ночи
    bool isDaytime() const;

    // сеттеры
    void set_cityName(const std::string& name);
    void set_temperatureKelvin(double kelvin);
    void set_humidity(int hum);
    void set_pressurePascal(double pascal);
    void set_windSpeed(double speed);
    void set_windDegrees(int deg);
    void set_windGust(double gust);
    void set_timezoneOffset(int offset);
    void set_measurementTime(long long time);
    void set_sunsetTime(long long time);
    void set_sunriseTime(long long time);
    void set_description(const std::string& desc);

    // переопределние всех полей
    void updateAll(
        std::optional<double> kelvin = std::nullopt,
        std::optional<int> hum = std::nullopt,
        std::optional<double> pascal = std::nullopt,
        std::optional<double> windSpd = std::nullopt,
        std::optional<int> windDeg = std::nullopt,
        std::optional<double> gust = std::nullopt,
        std::optional<int> tzOffset = std::nullopt,
        std::optional<long long> measTime = std::nullopt,
        std::optional<long long> sunset = std::nullopt,
        std::optional<long long> sunrise = std::nullopt,
        const std::string& desc = "Ќет данных.");
};


// основна€ функци€: получение данных о погоде дл€ города
Weather_Now GetWeatherCity(const std::string& city_name);


// ¬спомогательна€ структура дл€ передачи сырых данных от API
struct WeatherDataRaw {
    bool is_valid = false;              // флаг успешного получени€ данных
    std::string error_message;          // текст ошибки (если есть)

    // —ырые значени€ от API (в единицах API: K, Pa, м/с и т.д.)
    std::optional<double> temperature_kelvin;
    std::optional<int> humidity;
    std::optional<double> pressure_pascal;
    std::optional<double> wind_speed;
    std::optional<int> wind_degrees;
    std::optional<double> wind_gust;
    std::optional<long long> sunrise;
    std::optional<long long> sunset;
    std::optional<int> timezone_offset;
    std::optional<long long> measurement_time;
    std::string description;
};


// функци€ запроса к API (проверка и получение сырых данных)
WeatherDataRaw RequestToAPI(const std::string& city_name);


#endif //_WX_WEATHER_H
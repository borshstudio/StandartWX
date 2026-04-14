//
// Created by ASUS on 24.03.2026.
//

// При использовании с русским текстом рекомендуется установить локаль:
// setlocale(LC_ALL, "Russian");

#include "weather.h"

#include <cmath>
#include <ctime>
#include <stdexcept>

using std::string;
using std::optional;
using std::nullopt;


// перевод температуры из Кельвина в Цельсия
double WeatherUtils::kelvin_To_Celsius(double kelvin)
{
    return kelvin - KELVIN_OFFSET;
}

// перевод давления из Паскаль в мм рт ст
double WeatherUtils::pascal_To_MmHg(double pascal)
{
    return pascal * PASCAL_TO_MMHG;
}


// форматированный вывод
double WeatherUtils::formatValue(double value, Precision precision)
{
    if (precision == HUNDREDTHS)
        return std::round(value * 100.0) / 100.0;
    if (precision == TENTHS)
        return std::round(value * 10.0) / 10.0;
    if (precision == INTEGER)
        return std::round(value);
    return std::round(value);
}


// форматированный вывод времени
string WeatherUtils::formatTimestamp(long long timestamp, int timezoneOffset)
{
    time_t loc_time = (time_t)(timestamp + timezoneOffset);
    struct tm* time_info = std::gmtime(&loc_time);

    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M", time_info);

    return string(buffer);
}


// конструктор по умолчанию
Weather_Now::Weather_Now(const string& name) : cityName(name) {
    description = "Нет данных.";
}


// конструктор полноценный
Weather_Now::Weather_Now(
    const string& name,
    optional<double> kelvin,
    optional<int> hum,
    optional<double> pascal,
    optional<double> windSpd,
    optional<int> windDeg,
    optional<double> gust,
    optional<int> tzOffset,
    optional<long long> measTime,
    optional<long long> sunset,
    optional<long long> sunrise,
    const string& desc) :
    cityName(name),
    temperatureKelvin(kelvin),
    humidity(hum),
    pressurePascal(pascal),
    windSpeed(windSpd),
    windDegrees(windDeg),
    windGust(gust),
    timezoneOffset(tzOffset),
    measurementTime(measTime),
    sunsetTime(sunset),
    sunriseTime(sunrise),
    description(desc)
{
    if (temperatureKelvin.has_value()) {
        temperatureCelsius = WeatherUtils::kelvin_To_Celsius(temperatureKelvin.value());
    }

    if (pressurePascal.has_value()) {
        pressureMmHg = WeatherUtils::pascal_To_MmHg(pressurePascal.value());
    }
}

// выдать название города
string Weather_Now::get_cityName() const {
    return cityName;
}

// выдать температуру в кельвинах
optional<double> Weather_Now::get_temperatureKelvin(Precision precision) const {
    if (!temperatureKelvin.has_value()) return nullopt;
    return WeatherUtils::formatValue(temperatureKelvin.value(), precision);
}

// выдать температуру в цельсия
optional<double> Weather_Now::get_temperatureCelsius(Precision precision) const {
    if (!temperatureCelsius.has_value()) return nullopt;
    return WeatherUtils::formatValue(temperatureCelsius.value(), precision);
}

// выдать влажность
optional<int> Weather_Now::get_humidity() const {
    return humidity;
}

// выдать скорость ветра
optional<double> Weather_Now::get_windSpeed(Precision precision) const {
    if (!windSpeed.has_value()) return nullopt;
    return WeatherUtils::formatValue(windSpeed.value(), precision);
}

// выдать описание
string Weather_Now::get_description() const {
    return description;
}

// выдать направление ветра (текст)
string Weather_Now::get_windDirectionText() const {
    if (!windDegrees.has_value()) return "Неизвестно";

    int deg = windDegrees.value();
    if (deg >= 337.5 || deg < 22.5)   return "Северный";
    if (deg >= 22.5 && deg < 67.5)    return "Северо-восточный";
    if (deg >= 67.5 && deg < 112.5)   return "Восточный";
    if (deg >= 112.5 && deg < 157.5)  return "Юго-восточный";
    if (deg >= 157.5 && deg < 202.5)  return "Южный";
    if (deg >= 202.5 && deg < 247.5)  return "Юго-западный";
    if (deg >= 247.5 && deg < 292.5)  return "Западный";
    if (deg >= 292.5 && deg < 337.5)  return "Северо-западный";
    return "Неизвестно";
}

// выдать давление в паскалях
optional<double> Weather_Now::get_pressurePascal(Precision precision) const {
    if (!pressurePascal.has_value()) return nullopt;
    return WeatherUtils::formatValue(pressurePascal.value(), precision);
}

// выдать давление в мм рт ст
optional<double> Weather_Now::get_pressureMmHg(Precision precision) const {
    if (!pressureMmHg.has_value()) return nullopt;
    return WeatherUtils::formatValue(pressureMmHg.value(), precision);
}

// выдать порывы ветра
optional<double> Weather_Now::get_windGust(Precision precision) const {
    if (!windGust.has_value()) return nullopt;
    return WeatherUtils::formatValue(windGust.value(), precision);
}

// выдать время замера
string Weather_Now::get_measurementTime() const {
    if (!measurementTime.has_value() || !timezoneOffset.has_value()) {
        return "Нет данных.";
    }
    return WeatherUtils::formatTimestamp(measurementTime.value(), timezoneOffset.value());
}

// выдать время рассвета
string Weather_Now::get_sunriseTime() const {
    if (!sunriseTime.has_value() || !timezoneOffset.has_value()) {
        return "Нет данных.";
    }
    return WeatherUtils::formatTimestamp(sunriseTime.value(), timezoneOffset.value());
}

// выдать время заката
string Weather_Now::get_sunsetTime() const {
    if (!sunsetTime.has_value() || !timezoneOffset.has_value()) {
        return "Нет данных.";
    }
    return WeatherUtils::formatTimestamp(sunsetTime.value(), timezoneOffset.value());
}

// выдать часовой пояс
optional<int> Weather_Now::get_timezoneOffset() const {
    return timezoneOffset;
}

// проверка дня/ночи
bool Weather_Now::isDaytime() const {
    if (!measurementTime.has_value() || !sunriseTime.has_value() || !sunsetTime.has_value()) {
        return true;  // если данных нет, считаем что день
    }
    return (measurementTime.value() >= sunriseTime.value() &&
        measurementTime.value() <= sunsetTime.value());
}

// задать названия города
void Weather_Now::set_cityName(const string& name) {
    cityName = name;
}

// задать температуры
void Weather_Now::set_temperatureKelvin(double kelvin) {
    temperatureKelvin = kelvin;
    temperatureCelsius = WeatherUtils::kelvin_To_Celsius(kelvin);
    // TODO
}

// задать влажности
void Weather_Now::set_humidity(int hum) {
    humidity = hum;
    // TODO
}

// задать давления
void Weather_Now::set_pressurePascal(double pascal) {
    pressurePascal = pascal;
    pressureMmHg = WeatherUtils::pascal_To_MmHg(pascal);
    // TODO
}

// задать скорости ветра
void Weather_Now::set_windSpeed(double speed) {
    windSpeed = speed;
    // TODO
}

// задать направление ветра (в градусах)
void Weather_Now::set_windDegrees(int deg) {
    windDegrees = deg;
    // TODO
}

// задать порывы ветра
void Weather_Now::set_windGust(double gust) {
    windGust = gust;
    // TODO
}

// задать часового пояса
void Weather_Now::set_timezoneOffset(int offset) {
    timezoneOffset = offset;
    // TODO
}

// задать времени замера
void Weather_Now::set_measurementTime(long long time) {
    measurementTime = time;
    // TODO
}

// задать времени заката
void Weather_Now::set_sunsetTime(long long time) {
    sunsetTime = time;
    // TODO
}

// задать времени рассвета
void Weather_Now::set_sunriseTime(long long time) {
    sunriseTime = time;
    // TODO
}

// задать описания
void Weather_Now::set_description(const string& desc) {
    description = desc;
    // TODO
}

// задать новый комплект данных сразу
void Weather_Now::updateAll(
    optional<double> kelvin,
    optional<int> hum,
    optional<double> pascal,
    optional<double> windSpd,
    optional<int> windDeg,
    optional<double> gust,
    optional<int> tzOffset,
    optional<long long> measTime,
    optional<long long> sunset,
    optional<long long> sunrise,
    const string& desc)
{
    throw std::logic_error("Not implemented yet");
    // TODO
}
//
// Created by ASUS on 24.03.2026.
//

// При использовании с русским текстом рекомендуется установить локаль:
// setlocale(LC_ALL, "Russian");

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <string>
#include <optional>


using std::string;
using std::round;
using std::optional;
using std::nullopt;


constexpr double KELVIN_OFFSET = 273.15;
constexpr double PASCAL_TO_MMHG = 0.75006;


typedef enum { HUNDREDTHS = 3, INTEGER = 1, TENTHS = 2 } Precision;


namespace WeatherUtils {
    // перевод температуры из Кельвина в Цельсия
    double kelvin_To_Celsius(double kelvin)
    {
        return kelvin - KELVIN_OFFSET;
    }

    // перевод давления из Паскаль в мм рт ст
    double pascal_To_MmHg(double pascal)
    {
        return pascal * PASCAL_TO_MMHG;
    }


    // форматированный вывод
    double formatValue(double value, Precision precision)
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
    string formatTimestamp(long long timestamp, int timezoneOffset)
    {
        time_t loc_time = (time_t)(timestamp + timezoneOffset);
        struct tm* time_info = std::gmtime(&loc_time);

        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M", time_info);

        return string(buffer);
    }
}




// класс погоды_сейчас
class Weather_Now
{
private:
    string cityName;                              // название города

    optional<double> temperatureKelvin;           // температура в кельвинах
    optional<double> temperatureCelsius;          // температура в градусах цельсия
    optional<int> humidity;                       // влажность
    optional<double> pressurePascal;              // давление в паскалях
    optional<double> pressureMmHg;                // давление в мм

    optional<double> windSpeed;                   // скорость ветра
    optional<int> windDegrees;                    // направление ветра
    optional<double> windGust;                    // порывы ветра

    optional<int> timezoneOffset;                 // часовой пояс
    optional<long long> measurementTime;          // время замера
    optional<long long> sunsetTime, sunriseTime;  //время заката и рассвета

    string description;                           // описание


public:
    // конструктор по умолчанию
    Weather_Now(const string& name) : cityName(name) {
        description = "Нет данных.";
    }

    // конструктор полноценный
    Weather_Now(
        const string& name,
        optional<double> kelvin = nullopt,
        optional<int> hum = nullopt,
        optional<double> pascal = nullopt,
        optional<double> windSpd = nullopt,
        optional<int> windDeg = nullopt,
        optional<double> gust = nullopt,
        optional<int> tzOffset = nullopt,
        optional<long long> measTime = nullopt,
        optional<long long> sunset = nullopt,
        optional<long long> sunrise = nullopt,
        const string& desc = "Нет данных.") :
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
    string get_cityName() const {
        return cityName;
    }

    // выдать температуру в кельвинах
    optional<double> get_temperatureKelvin(Precision precision = TENTHS) const {
        if (!temperatureKelvin.has_value()) return nullopt;
        return WeatherUtils::formatValue(temperatureKelvin.value(), precision);
    }

    // выдать температуру в цельсия
    optional<double> get_temperatureCelsius(Precision precision = TENTHS) const {
        if (!temperatureCelsius.has_value()) return nullopt;
        return WeatherUtils::formatValue(temperatureCelsius.value(), precision);
    }

    // выдать влажность
    optional<int> get_humidity() const {
        return humidity;
    }

    // выдать скорость ветра
    optional<double> get_windSpeed(Precision precision = TENTHS) const {
        if (!windSpeed.has_value()) return nullopt;
        return WeatherUtils::formatValue(windSpeed.value(), precision);
    }

    // выдать описание
    string get_description() const {
        return description;
    }

    // выдать направление ветра (текст)
    string get_windDirectionText() const {
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
    optional<double> get_pressurePascal(Precision precision = INTEGER) const {
        if (!pressurePascal.has_value()) return nullopt;
        return WeatherUtils::formatValue(pressurePascal.value(), precision);
    }

    // выдать давление в мм рт ст
    optional<double> get_pressureMmHg(Precision precision = TENTHS) const {
        if (!pressureMmHg.has_value()) return nullopt;
        return WeatherUtils::formatValue(pressureMmHg.value(), precision);
    }

    // выдать порывы ветра
    optional<double> get_windGust(Precision precision = TENTHS) const {
        if (!windGust.has_value()) return nullopt;
        return WeatherUtils::formatValue(windGust.value(), precision);
    }

    // выдать время замера
    string get_measurementTime() const {
        if (!measurementTime.has_value() || !timezoneOffset.has_value()) {
            return "Нет данных.";
        }
        return WeatherUtils::formatTimestamp(measurementTime.value(), timezoneOffset.value());
    }

    // выдать время рассвета
    string get_sunriseTime() const {
        if (!sunriseTime.has_value() || !timezoneOffset.has_value()) {
            return "Нет данных.";
        }
        return WeatherUtils::formatTimestamp(sunriseTime.value(), timezoneOffset.value());
    }

    // выдать время заката
    string get_sunsetTime() const {
        if (!sunsetTime.has_value() || !timezoneOffset.has_value()) {
            return "Нет данных.";
        }
        return WeatherUtils::formatTimestamp(sunsetTime.value(), timezoneOffset.value());
    }

    // выдать часовой пояс
    optional<int> get_timezoneOffset() const {
        return timezoneOffset;
    }

    // проверка дня/ночи
    bool isDaytime() const {
        if (!measurementTime.has_value() || !sunriseTime.has_value() || !sunsetTime.has_value()) {
            return true;  // если данных нет, считаем что день
        }
        return (measurementTime.value() >= sunriseTime.value() &&
            measurementTime.value() <= sunsetTime.value());
    }

    // задать названия города
    void set_cityName(const string& name) {
        cityName = name;
    }

    // задать температуры
    void set_temperatureKelvin(double kelvin) {
        temperatureKelvin = kelvin;
        temperatureCelsius = WeatherUtils::kelvin_To_Celsius(kelvin);
    }

    // задать влажности
    void set_humidity(int hum) {
        humidity = hum;
    }

    // задать давления
    void set_pressurePascal(double pascal) {
        pressurePascal = pascal;
        pressureMmHg = WeatherUtils::pascal_To_MmHg(pascal);
    }

    // задать скорости ветра
    void set_windSpeed(double speed) {
        windSpeed = speed;
    }

    // задать направления ветра
    void set_windGust(double gust) {
        windGust = gust;
    }

    // задать часового пояса
    void set_timezoneOffset(int offset) {
        timezoneOffset = offset;
    }

    // задать времени замера
    void set_measurementTime(long long time) {
        measurementTime = time;
    }

    // задать времени заката
    void set_sunsetTime(long long time) {
        sunsetTime = time;
    }

    // задать времени рассвета
    void set_sunriseTime(long long time) {
        sunriseTime = time;
    }

    // задать описания
    void set_description(const string& desc) {
        description = desc;
    }
};




// класс прогноза_погоды
class Weather_Forecast : public Weather_Now
{
private:
    optional<double> minTemperatureKelvin;     // мин. температура в кельвинах
    optional<double> maxTemperatureKelvin;     // макс. температура в кельвинах
    optional<double> minTemperatureCelsius;    // мин. температура в цельсиях
    optional<double> maxTemperatureCelsius;    // макс. температура в цельсиях
    optional<double> precipitationProbability; // вероятность осадков (0-1). выводит в %
    optional<long long> forecastTime;          // время прогноза


public:
    // конструктор по умолчанию
    Weather_Forecast(const string& name) : Weather_Now(name) {}

    // полноценный конструктор
    Weather_Forecast(
        const string& name,
        optional<double> kelvin = nullopt,
        optional<int> hum = nullopt,
        optional<double> pascal = nullopt,
        optional<double> windSpd = nullopt,
        optional<int> windDeg = nullopt,
        optional<double> gust = nullopt,
        optional<int> tzOffset = nullopt,
        optional<long long> measTime = nullopt,
        optional<long long> sunset = nullopt,
        optional<long long> sunrise = nullopt,
        optional<double> minKelvin = nullopt,
        optional<double> maxKelvin = nullopt,
        optional<double> precipProb = nullopt,
        optional<long long> foreTime = nullopt,
        const string& desc = "Прогноз погоды."
    ) : Weather_Now(name, kelvin, hum, pascal, windSpd, windDeg, gust,
        tzOffset, measTime, sunset, sunrise, desc),
        minTemperatureKelvin(minKelvin),
        maxTemperatureKelvin(maxKelvin),
        precipitationProbability(precipProb),
        forecastTime(foreTime)
    {
        if (minTemperatureKelvin.has_value()) {
            minTemperatureCelsius = WeatherUtils::kelvin_To_Celsius(minTemperatureKelvin.value());
        }
        if (maxTemperatureKelvin.has_value()) {
            maxTemperatureCelsius = WeatherUtils::kelvin_To_Celsius(maxTemperatureKelvin.value());
        }
    }

    // выдать минимальную температуру в кельвинах
    optional<double> get_minTemperatureKelvin(Precision precision = TENTHS) const {
        if (!minTemperatureKelvin.has_value()) return nullopt;
        return WeatherUtils::formatValue(minTemperatureKelvin.value(), precision);
    }

    // выдать максимальную темпераутру в кельвинах
    optional<double> get_maxTemperatureKelvin(Precision precision = TENTHS) const {
        if (!maxTemperatureKelvin.has_value()) return nullopt;
        return WeatherUtils::formatValue(maxTemperatureKelvin.value(), precision);
    }

    // выдать минимальную температуру в цельсия
    optional<double> get_minTemperatureCelsius(Precision precision = TENTHS) const {
        if (!minTemperatureCelsius.has_value()) return nullopt;
        return WeatherUtils::formatValue(minTemperatureCelsius.value(), precision);
    }

    // выдать максимальную температуру в цельсия
    optional<double> get_maxTemperatureCelsius(Precision precision = TENTHS) const {
        if (!maxTemperatureCelsius.has_value()) return nullopt;
        return WeatherUtils::formatValue(maxTemperatureCelsius.value(), precision);
    }

    // выдать вероятность осадков в %
    optional<double> get_precipitationProbability() const {
        if (!precipitationProbability.has_value()) return nullopt;
        return precipitationProbability.value() * 100;
    }

    // выдать время прогноза
    string get_forecastTime() const {
        if (!forecastTime.has_value() || !get_timezoneOffset().has_value()) {
            return "Нет данных.";
        }
        return WeatherUtils::formatTimestamp(forecastTime.value(), get_timezoneOffset().value());
    }

    // задать минимальную температуру (ввод в кельвинах - задается для обеих температур)
    void set_minTemperature(double kelvin) {
        minTemperatureKelvin = kelvin;
        minTemperatureCelsius = WeatherUtils::kelvin_To_Celsius(kelvin);
    }

    // задать максимальную температуру (ввод в кельвинах - задается для обеих температур) 
    void set_maxTemperature(double kelvin) {
        maxTemperatureKelvin = kelvin;
        maxTemperatureCelsius = WeatherUtils::kelvin_To_Celsius(kelvin);
    }

    // задать вероятность осадков
    void set_precipitationProbability(double prob) {
        precipitationProbability = prob;
    }

    // задать время замеров
    void set_forecastTime(long long time) {
        forecastTime = time;
    }
};




// класс погодная_угроза
class Weather_Alert
{
private:
    optional<string> senderName;    // название источника оповещения
    optional<string> eventName;     // название угрозы
    optional<long long> startTime;  // дата и время начала
    optional<long long> endTime;    // дата и время окончания
    optional<string> description;   // описание оповещения
    optional<string> tags;          // тип угрозы
    optional<int> timezoneOffset;   // часовой пояс


public:
    // конструктор по умолчанию
    Weather_Alert() = default;
    // конструктор со всеми параметрами
    Weather_Alert(
        optional<string> sender = nullopt,
        optional<string> event = nullopt,
        optional<long long> start = nullopt,
        optional<long long> end = nullopt,
        optional<string> desc = nullopt,
        optional<string> tag = nullopt,
        optional<int> timeOff = nullopt
    ) : senderName(sender),
        eventName(event),
        startTime(start),
        endTime(end),
        description(desc),
        tags(tag),
        timezoneOffset(timeOff) {
    }

    // вывод названия источника оповещения
    optional<string> get_senderName() const {
        return senderName;
    }

    // вывод названия угрозы
    optional<string> get_eventName() const {
        return eventName;
    }

    // вывод даты и времени начала
    string get_startTime() const {
        if (!startTime.has_value() || !timezoneOffset.has_value()) {
            return "Нет данных.";
        }
        return WeatherUtils::formatTimestamp(startTime.value(), timezoneOffset.value());
    }

    // вывод даты и времени окончания
    string get_endTime() const {
        if (!endTime.has_value() || !timezoneOffset.has_value()) {
            return "Нет данных";
        }
        return WeatherUtils::formatTimestamp(endTime.value(), timezoneOffset.value());
    }

    // вывод описания угрозы
    optional<string> get_description() const {
        return description;
    }

    // вывод типа угрозы
    optional<string> get_tags() const {
        return tags;
    }

    // задать название источника
    void set_senderName(const string& name) {
        senderName = name;
    }

    // задать название угрозы
    void set_eventName(const string& name) {
        eventName = name;
    }

    // задать дату и время начала
    void set_startTime(long long time) {
        startTime = time;
    }

    // задать дату и время окончания
    void set_endTime(long long time) {
        endTime = time;
    }

    // задать часовой пояс
    void set_timezoneOffset(int timezone) {
        timezoneOffset = timezone;
    }

    // задать описание
    void set_description(const string& desc) {
        description = desc;
    }

    // задать тип угрозы
    void set_tags(const string& tag) {
        tags = tag;
    }

    // есть ли угроза сейчас
    bool is_alert_now(long long timeNow) const {
        if (!startTime.has_value() || !endTime.has_value()) return false;
        return (startTime.value() <= timeNow && timeNow <= endTime.value());
    }
};
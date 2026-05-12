#include "weather.h"
#include "api.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>


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

        city = translateCityToEnglish(city);

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
        if (choice == "1") {
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
                city = translateCityToEnglish(city);
                if (isCityInFavorites(city)) {
                    std::cout << "[!] Этот город уже в избранном.\n";
                }
                else {
                    favorites.push_back(city);
                    saveFavorites(favorites);
                    std::cout << "[OK] Город добавлен.\n";
                }
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
            int idx = safeStoi(n) - 1;
            if (idx >= 0 && idx < (int)favorites.size()) {
                favorites.erase(favorites.begin() + idx);
                saveFavorites(favorites);
                std::cout << "[OK] Город удален.\n";
            }
            else {
                std::cout << "[ERROR] Неверный номер.\n";
            }
        }
        else if (choice == "4") {
            if (favorites.empty()) continue;
            std::cout << "Введите номер: ";
            std::string n;
            std::getline(std::cin, n);
            int idx = safeStoi(n) - 1;
            if (idx >= 0 && idx < (int)favorites.size()) {
                showWeatherForCity(favorites[idx], apiKey);
            }
            else {
                std::cout << "[ERROR] Неверный номер.\n";
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



std::string translateCityToEnglish(const std::string& input) {
    bool hasCyrillic = false;
    for (char c : input) {
        if ((c >= 'А' && c <= 'я') || c == 'Ё' || c == 'ё') {
            hasCyrillic = true;
            break;
        }
    }
    if (!hasCyrillic) return input;

    static const std::unordered_map<std::string, std::string> cityDict = {
        {"Москва", "Moscow"},
        {"Санкт-Петербург", "Saint Petersburg"},
        {"Санкт Петербург", "Saint Petersburg"},
        {"Питер", "Saint Petersburg"},
        {"Новосибирск", "Novosibirsk"},
        {"Екатеринбург", "Yekaterinburg"},
        {"Нижний Новгород", "Nizhny Novgorod"},
        {"Казань", "Kazan"},
        {"Челябинск", "Chelyabinsk"},
        {"Омск", "Omsk"},
        {"Самара", "Samara"},
        {"Уфа", "Ufa"},
        {"Красноярск", "Krasnoyarsk"},
        {"Пермь", "Perm"},
        {"Воронеж", "Voronezh"},
        {"Волгоград", "Volgograd"},
        {"Краснодар", "Krasnodar"},
        {"Саратов", "Saratov"},
        {"Тюмень", "Tyumen"},
        {"Ижевск", "Izhevsk"},
        {"Барнаул", "Barnaul"},
        {"Иркутск", "Irkutsk"},
        {"Хабаровск", "Khabarovsk"},
        {"Ярославль", "Yaroslavl"},
        {"Владивосток", "Vladivostok"},
        {"Томск", "Tomsk"},
        {"Кемерово", "Kemerovo"},
        {"Рязань", "Ryazan"},
        {"Астрахань", "Astrakhan"},
        {"Пенза", "Penza"},
        {"Киров", "Kirov"},
        {"Калининград", "Kaliningrad"},
        {"Тула", "Tula"},
        {"Тверь", "Tver"},
        {"Сочи", "Sochi"},
        {"Мурманск", "Murmansk"},
        {"Париж", "Paris"},
        {"Лондон", "London"},
        {"Рим", "Rome"},
        {"Берлин", "Berlin"},
        {"Мадрид", "Madrid"},
        {"Пекин", "Beijing"},
        {"Токио", "Tokyo"},
        {"Минск", "Minsk"},
        {"Киев", "Kyiv"},
        {"Астана", "Astana"}
    };

    auto it = cityDict.find(input);
    if (it != cityDict.end()) 
        return it->second;

    static const std::unordered_map<char, std::string> translit = {
        {'А',"A"},{'а',"a"},{'Б',"B"},{'б',"b"},{'В',"V"},{'в',"v"},
        {'Г',"G"},{'г',"g"},{'Д',"D"},{'д',"d"},{'Е',"E"},{'е',"e"},
        {'Ё',"Yo"},{'ё',"yo"},{'Ж',"Zh"},{'ж',"zh"},{'З',"Z"},{'з',"z"},
        {'И',"I"},{'и',"i"},{'Й',"Y"},{'й',"y"},{'К',"K"},{'к',"k"},
        {'Л',"L"},{'л',"l"},{'М',"M"},{'м',"m"},{'Н',"N"},{'н',"n"},
        {'О',"O"},{'о',"o"},{'П',"P"},{'п',"p"},{'Р',"R"},{'р',"r"},
        {'С',"S"},{'с',"s"},{'Т',"T"},{'т',"t"},{'У',"U"},{'у',"u"},
        {'Ф',"F"},{'ф',"f"},{'Х',"Kh"},{'х',"kh"},{'Ц',"Ts"},{'ц',"ts"},
        {'Ч',"Ch"},{'ч',"ch"},{'Ш',"Sh"},{'ш',"sh"},{'Щ',"Shch"},{'щ',"shch"},
        {'Ъ',""},{'ъ',""},{'Ы',"Y"},{'ы',"y"},{'Ь',""},{'ь',""},
        {'Э',"E"},{'э',"e"},{'Ю',"Yu"},{'ю',"yu"},{'Я',"Ya"},{'я',"ya"}
    };

    std::string result;
    for (char c : input) {
        auto t = translit.find(c);
        result += (t != translit.end()) ? t->second : std::string(1, c);
    }
    return result;
}



bool isCityInFavorites(const std::string& city) {
    auto favorites = loadFavorites();
    for (const auto& fav : favorites) {
        if (fav == city) 
            return true;
    }
    return false;
}



int safeStoi(const std::string& str, int fallback = -1) {
    try {
        return std::stoi(str);
    }
    catch (...) {
        return fallback;
    }
}
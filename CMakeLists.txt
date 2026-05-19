cmake_minimum_required(VERSION 3.20)

project(WeatherApp)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

set(CPR_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CPR_FORCE_USE_SYSTEM_CURL OFF CACHE BOOL "" FORCE)
set(CPR_USE_SYSTEM_CURL OFF CACHE BOOL "" FORCE)
set(CPR_ENABLE_SSL ON CACHE BOOL "" FORCE)

FetchContent_Declare(
        cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 1.11.0
)

FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_MakeAvailable(cpr)
FetchContent_MakeAvailable(json)

add_executable(WeatherApp
        main.cpp
        api.cpp
        api.h
        weather.cpp
        weather.h
        utils.cpp
        utils.h
)

target_link_libraries(WeatherApp PRIVATE
        cpr::cpr
        nlohmann_json::nlohmann_json
)

if (WIN32)
    add_custom_command(TARGET WeatherApp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:WeatherApp>
            $<TARGET_FILE_DIR:WeatherApp>
            COMMAND_EXPAND_LISTS
    )
endif()


if (MSVC)
    target_compile_options(WeatherApp PRIVATE /utf-8)
endif()

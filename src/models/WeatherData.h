#pragma once
#include <Arduino.h>

// -----------------------------------------------------------------------
// WeatherData — current weather conditions for a location
// -----------------------------------------------------------------------
struct WeatherData {
    String   location;        // City / location name
    float    temperatureC;    // Temperature in Celsius
    float    feelsLikeC;      // "Feels like" temperature
    float    humidity;        // Relative humidity 0–100 %
    float    windSpeedKmh;    // Wind speed
    String   description;     // Human-readable: "light rain", "clear sky"
    String   iconCode;        // Provider icon code (e.g. OpenWeatherMap "01d")
    uint32_t fetchedAtMs;     // millis() when data was fetched
    bool     valid = false;   // False until first successful fetch
};

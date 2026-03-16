#pragma once
#include <Arduino.h>
#include "models/WeatherData.h"
#include "models/Result.h"

// -----------------------------------------------------------------------
// IWeatherProvider — abstraction for weather data retrieval
//
// Concrete implementations (to be added):
//   - OpenWeatherMapProvider  (api.openweathermap.org)
//   - WeatherApiProvider      (weatherapi.com)
// -----------------------------------------------------------------------
class IWeatherProvider {
public:
    virtual ~IWeatherProvider() = default;

    virtual bool init() = 0;

    // Trigger a weather fetch for the given location.
    // location = city name, "lat,lon", or "auto" for IP-based.
    // May be blocking (HTTP request) — consider calling from scheduler.
    virtual bool fetchWeather(const char* location) = 0;

    virtual WeatherData getLastWeather() const = 0;

    // True if data was successfully fetched and not too old
    virtual bool isDataFresh() const = 0;

    // Provider name for logging
    virtual const char* getName() const = 0;
};

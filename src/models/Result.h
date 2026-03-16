#pragma once
#include <Arduino.h>

// -----------------------------------------------------------------------
// Status — lightweight success/failure result with optional error message
// Used for operations that don't return a value (void operations).
// -----------------------------------------------------------------------
struct Status {
    bool   success;
    String errorMessage;

    static Status ok()                        { return {true,  ""}; }
    static Status error(const String& msg)    { return {false, msg}; }

    explicit operator bool() const { return success; }
};

// -----------------------------------------------------------------------
// Result<T> — success/failure result that also carries a value.
// Use for operations that return data on success.
//
// Example:
//   Result<WeatherData> r = provider->fetchWeather("London");
//   if (r) { use(r.value); }
// -----------------------------------------------------------------------
template<typename T>
struct Result {
    bool   success;
    T      value;
    String errorMessage;

    static Result<T> ok(const T& val) {
        Result<T> r;
        r.success      = true;
        r.value        = val;
        return r;
    }

    static Result<T> error(const String& msg) {
        Result<T> r;
        r.success      = false;
        r.errorMessage = msg;
        return r;
    }

    explicit operator bool() const { return success; }
};

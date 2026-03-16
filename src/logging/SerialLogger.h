#pragma once
#include "ILogger.h"
#include <Arduino.h>

// -----------------------------------------------------------------------
// SerialLogger — ILogger implementation that writes to Arduino Serial
// -----------------------------------------------------------------------
class SerialLogger : public ILogger {
public:
    explicit SerialLogger(uint32_t baudRate = 115200,
                          LogLevel minLevel = LogLevel::DEBUG);

    void init() override;
    void log(LogLevel level, const char* tag, const char* message) override;
    void logf(LogLevel level, const char* tag, const char* format, ...) override;

    // Change minimum log level at runtime
    void setMinLevel(LogLevel level);

private:
    uint32_t  _baudRate;
    LogLevel  _minLevel;

    static const char* _levelLabel(LogLevel level);
};

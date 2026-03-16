#include "SerialLogger.h"
#include <stdarg.h>

SerialLogger::SerialLogger(uint32_t baudRate, LogLevel minLevel)
    : _baudRate(baudRate), _minLevel(minLevel) {}

void SerialLogger::init() {
    Serial.begin(_baudRate);
    while (!Serial && millis() < 3000) { /* wait up to 3 s for USB CDC */ }
    Serial.println("[SerialLogger] Logging initialized.");
}

void SerialLogger::log(LogLevel level, const char* tag, const char* message) {
    if (level < _minLevel) return;

    // Format: [TIME_MS][LEVEL][TAG] message
    Serial.printf("[%8lu][%s][%s] %s\n",
                  millis(),
                  _levelLabel(level),
                  tag ? tag : "?",
                  message ? message : "");
}

void SerialLogger::logf(LogLevel level, const char* tag, const char* format, ...) {
    if (level < _minLevel) return;
    if (!format) return;

    char buf[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);

    log(level, tag, buf);
}

void SerialLogger::setMinLevel(LogLevel level) {
    _minLevel = level;
}

const char* SerialLogger::_levelLabel(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:   return "DBG";
        case LogLevel::INFO:    return "INF";
        case LogLevel::WARNING: return "WRN";
        case LogLevel::ERROR:   return "ERR";
        default:                return "???";
    }
}

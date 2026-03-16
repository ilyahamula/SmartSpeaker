#pragma once
#include <Arduino.h>

// Log severity levels
enum class LogLevel : uint8_t {
    DEBUG   = 0,
    INFO    = 1,
    WARNING = 2,
    ERROR   = 3
};

// -----------------------------------------------------------------------
// ILogger — abstract logging interface
//
// Concrete implementations: SerialLogger (Serial output),
// future: BLELogger, SDCardLogger, SyslogLogger, etc.
// -----------------------------------------------------------------------
class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void init() = 0;

    // Core log methods (must implement)
    virtual void log(LogLevel level, const char* tag, const char* message) = 0;

    // Formatted log (printf-style)
    virtual void logf(LogLevel level, const char* tag, const char* format, ...) = 0;

    // Convenience shorthands — call through to log()
    void debug(const char* tag, const char* msg) { log(LogLevel::DEBUG,   tag, msg); }
    void info (const char* tag, const char* msg) { log(LogLevel::INFO,    tag, msg); }
    void warn (const char* tag, const char* msg) { log(LogLevel::WARNING, tag, msg); }
    void error(const char* tag, const char* msg) { log(LogLevel::ERROR,   tag, msg); }
};

// Null-safe logging macros — safe even if logger pointer is nullptr
#define LOG_DEBUG(L, TAG, MSG)  do { if (L) (L)->debug(TAG, MSG); } while(0)
#define LOG_INFO(L,  TAG, MSG)  do { if (L) (L)->info (TAG, MSG); } while(0)
#define LOG_WARN(L,  TAG, MSG)  do { if (L) (L)->warn (TAG, MSG); } while(0)
#define LOG_ERROR(L, TAG, MSG)  do { if (L) (L)->error(TAG, MSG); } while(0)

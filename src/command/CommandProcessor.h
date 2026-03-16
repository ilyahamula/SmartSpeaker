#pragma once
#include <Arduino.h>
#include "ICommandSource.h"
#include "models/Command.h"
#include "logging/ILogger.h"

// Forward declarations — avoid pulling all headers into this interface
class AudioManager;
class IWeatherProvider;
class INewsProvider;
class IAssistantProvider;

// -----------------------------------------------------------------------
// CommandProcessor — polls all registered command sources each loop
// and routes each Command to the appropriate subsystem handler.
//
// Dependencies are injected via setters so concrete implementations
// can be swapped at wiring time (in Application or main.cpp).
// -----------------------------------------------------------------------
class CommandProcessor {
public:
    explicit CommandProcessor(ILogger* logger);
    ~CommandProcessor() = default;

    void init();
    void update();   // Call from main loop — polls sources, processes queue

    // --- Dependency injection ---
    // Register up to MAX_SOURCES command input channels
    bool addCommandSource(ICommandSource* source);

    // Plug in subsystem handlers
    void setAudioManager(AudioManager* manager);
    void setWeatherProvider(IWeatherProvider* provider);
    void setNewsProvider(INewsProvider* provider);
    void setAssistantProvider(IAssistantProvider* provider);

private:
    // Routing / dispatch
    void _dispatch(const Command& cmd);
    void _handlePlayMusic(const Command& cmd);
    void _handleVolumeChange(const Command& cmd);
    void _handleWeatherRequest(const Command& cmd);
    void _handleNewsRequest(const Command& cmd);
    void _handleQuestion(const Command& cmd);
    void _handleSystemCommand(const Command& cmd);

    static constexpr uint8_t MAX_SOURCES = 6;

    ICommandSource*     _sources[MAX_SOURCES];
    uint8_t             _sourceCount = 0;

    AudioManager*       _audioManager       = nullptr;
    IWeatherProvider*   _weatherProvider    = nullptr;
    INewsProvider*      _newsProvider       = nullptr;
    IAssistantProvider* _assistantProvider  = nullptr;

    ILogger*            _logger;
};

#pragma once
#include <Arduino.h>

// Forward declarations — concrete types included in Application.cpp
class ILogger;
class IConnectivityManager;
class IWeatherProvider;
class INewsProvider;
class IAssistantProvider;
class ICommandSource;
class IAudioPlayer;
class IDisplay;
class IScheduler;
class CommandProcessor;
class AudioManager;

#include "models/DeviceState.h"

// -----------------------------------------------------------------------
// Application — top-level orchestrator for the SmartSpeaker firmware
//
// Responsibilities:
//   - Own and initialize all subsystem objects
//   - Wire injected concrete implementations to internal coordinators
//   - Drive the main loop: update connectivity, process commands,
//     update audio, run scheduler, update display
//
// Extension pattern:
//   Call the inject* / add* setters BEFORE calling setup().
//   To add a concrete implementation later, create it in main.cpp and
//   inject it — no changes to Application are required.
// -----------------------------------------------------------------------
class Application {
public:
    Application();
    ~Application();

    // ---------------------------------------------------------------
    // Dependency injection — call before setup()
    // ---------------------------------------------------------------
    void setConnectivityManager(IConnectivityManager* manager);
    void addCommandSource(ICommandSource* source);
    void addAudioPlayer(IAudioPlayer* player);
    void setWeatherProvider(IWeatherProvider* provider);
    void setNewsProvider(INewsProvider* provider);
    void setAssistantProvider(IAssistantProvider* provider);
    void setDisplay(IDisplay* display);

    // ---------------------------------------------------------------
    // Lifecycle — called from main.cpp
    // ---------------------------------------------------------------
    void setup();
    void loop();

    // ---------------------------------------------------------------
    // State access
    // ---------------------------------------------------------------
    const DeviceState& getDeviceState() const { return _deviceState; }
    ILogger*           getLogger()      const { return _logger; }

private:
    void _setupScheduledTasks();
    void _updateDeviceState();

    // --- Infrastructure (owned, concrete) ---
    ILogger*          _logger;       // SerialLogger instance
    IScheduler*       _scheduler;    // SimpleScheduler instance
    CommandProcessor* _commandProcessor;
    AudioManager*     _audioManager;

    // --- Pluggable dependencies (abstract, injected from outside) ---
    IConnectivityManager* _connectivity      = nullptr;
    IWeatherProvider*     _weatherProvider   = nullptr;
    INewsProvider*        _newsProvider      = nullptr;
    IAssistantProvider*   _assistantProvider = nullptr;
    IDisplay*             _display           = nullptr;

    // --- Application state ---
    DeviceState _deviceState;
};

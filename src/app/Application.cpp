#include "Application.h"

#include "logging/SerialLogger.h"
#include "scheduler/SimpleScheduler.h"
#include "command/CommandProcessor.h"
#include "audio/AudioManager.h"
#include "connectivity/IConnectivityManager.h"
#include "content/IWeatherProvider.h"
#include "content/INewsProvider.h"
#include "content/IAssistantProvider.h"
#include "hal/IDisplay.h"
#include "Config.h"

static const char* TAG = "Application";

// ============================================================
// Scheduled task definitions (inner structs, local to this TU)
// ============================================================

struct ConnectivityCheckTask : public ISchedulerTask {
    IConnectivityManager* connectivity;
    ILogger*              logger;

    ConnectivityCheckTask(IConnectivityManager* c, ILogger* l)
        : connectivity(c), logger(l) {}

    const char* getName()       const override { return "ConnectivityCheck"; }
    uint32_t    getIntervalMs() const override { return SCHEDULER_CONNECTIVITY_CHECK_MS; }

    void execute() override {
        if (!connectivity) return;
        connectivity->update();
        if (!connectivity->isNetworkAvailable()) {
            LOG_WARN(logger, "Connectivity", "Network unavailable — attempting reconnect.");
        }
    }
};

struct WeatherUpdateTask : public ISchedulerTask {
    IWeatherProvider* provider;
    ILogger*          logger;

    WeatherUpdateTask(IWeatherProvider* p, ILogger* l) : provider(p), logger(l) {}

    const char* getName()       const override { return "WeatherUpdate"; }
    uint32_t    getIntervalMs() const override { return SCHEDULER_WEATHER_UPDATE_MS; }

    void execute() override {
        if (!provider) return;
        LOG_INFO(logger, "WeatherUpdate", "Fetching weather data...");
        provider->fetchWeather(WEATHER_DEFAULT_LOCATION);
    }
};

struct NewsUpdateTask : public ISchedulerTask {
    INewsProvider* provider;
    ILogger*       logger;

    NewsUpdateTask(INewsProvider* p, ILogger* l) : provider(p), logger(l) {}

    const char* getName()       const override { return "NewsUpdate"; }
    uint32_t    getIntervalMs() const override { return SCHEDULER_NEWS_UPDATE_MS; }

    void execute() override {
        if (!provider) return;
        LOG_INFO(logger, "NewsUpdate", "Fetching news headlines...");
        provider->fetchNews(NEWS_DEFAULT_MAX_ITEMS);
    }
};

struct HeartbeatTask : public ISchedulerTask {
    ILogger*     logger;
    DeviceState* state;

    HeartbeatTask(ILogger* l, DeviceState* s) : logger(l), state(s) {}

    const char* getName()       const override { return "Heartbeat"; }
    uint32_t    getIntervalMs() const override { return SCHEDULER_STATUS_HEARTBEAT_MS; }

    void execute() override {
        if (!state) return;
        state->uptimeMs = millis();
        char buf[64];
        snprintf(buf, sizeof(buf), "Uptime: %lu s | Volume: %d%%",
                 state->uptimeMs / 1000, state->volume);
        LOG_DEBUG(logger, "Heartbeat", buf);
    }
};

// ============================================================
// Task instances (kept alive for the lifetime of Application)
// ============================================================
static ConnectivityCheckTask* s_connectivityTask = nullptr;
static WeatherUpdateTask*     s_weatherTask      = nullptr;
static NewsUpdateTask*        s_newsTask         = nullptr;
static HeartbeatTask*         s_heartbeatTask    = nullptr;

// ============================================================
// Application implementation
// ============================================================

Application::Application() {
    _logger           = new SerialLogger(APP_SERIAL_BAUD_RATE, LogLevel::DEBUG);
    _scheduler        = new SimpleScheduler(_logger);
    _commandProcessor = new CommandProcessor(_logger);
    _audioManager     = new AudioManager(_logger);

    _deviceState.status = DeviceStatus::BOOTING;
    _deviceState.volume = AUDIO_DEFAULT_VOLUME;
}

Application::~Application() {
    delete s_connectivityTask;
    delete s_weatherTask;
    delete s_newsTask;
    delete s_heartbeatTask;

    delete _audioManager;
    delete _commandProcessor;
    delete _scheduler;
    delete _logger;
}

// --- Dependency injection ---

void Application::setConnectivityManager(IConnectivityManager* manager) {
    _connectivity = manager;
}

void Application::addCommandSource(ICommandSource* source) {
    _commandProcessor->addCommandSource(source);
}

void Application::addAudioPlayer(IAudioPlayer* player) {
    _audioManager->addPlayer(player);
}

void Application::setWeatherProvider(IWeatherProvider* provider) {
    _weatherProvider = provider;
    _commandProcessor->setWeatherProvider(provider);
}

void Application::setNewsProvider(INewsProvider* provider) {
    _newsProvider = provider;
    _commandProcessor->setNewsProvider(provider);
}

void Application::setAssistantProvider(IAssistantProvider* provider) {
    _assistantProvider = provider;
    _commandProcessor->setAssistantProvider(provider);
}

void Application::setDisplay(IDisplay* display) {
    _display = display;
}

// --- Lifecycle ---

void Application::setup() {
    _logger->init();
    _logger->info(TAG, "=== SmartSpeaker booting ===");

    _deviceState.status = DeviceStatus::INITIALIZING;

    // Initialise connectivity (if injected)
    if (_connectivity) {
        if (!_connectivity->init()) {
            LOG_ERROR(_logger, TAG, "Connectivity manager init failed.");
        }
    } else {
        LOG_WARN(_logger, TAG, "No ConnectivityManager injected — running offline.");
    }

    // Initialise audio subsystem
    _audioManager->init();
    _audioManager->setVolume(_deviceState.volume);

    // Initialise command processor
    _commandProcessor->init();
    _commandProcessor->setAudioManager(_audioManager);

    // Initialise display (if injected)
    if (_display) {
        if (!_display->init()) {
            LOG_WARN(_logger, TAG, "Display init failed — continuing without display.");
        } else {
            _display->showStatus("Booting...");
        }
    }

    // Register periodic scheduled tasks
    _setupScheduledTasks();

    _deviceState.status = DeviceStatus::IDLE;
    _logger->info(TAG, "=== SmartSpeaker ready ===");
}

void Application::loop() {
    // 1. Update connectivity state
    if (_connectivity) {
        _connectivity->update();
    }

    // 2. Poll command sources and dispatch commands
    _commandProcessor->update();

    // 3. Drive audio playback (buffering, decoding, etc.)
    _audioManager->update();

    // 4. Run periodic scheduled tasks
    _scheduler->update();

    // 5. Refresh display
    if (_display) {
        _display->showPlaybackState(_audioManager->getState());
        _display->update();
    }

    // 6. Update device state snapshot
    _updateDeviceState();

    delay(APP_LOOP_DELAY_MS);
}

// --- Private helpers ---

void Application::_setupScheduledTasks() {
    s_heartbeatTask = new HeartbeatTask(_logger, &_deviceState);
    _scheduler->addTask(s_heartbeatTask);

    if (_connectivity) {
        s_connectivityTask = new ConnectivityCheckTask(_connectivity, _logger);
        _scheduler->addTask(s_connectivityTask);
    }

    if (_weatherProvider) {
        s_weatherTask = new WeatherUpdateTask(_weatherProvider, _logger);
        _scheduler->addTask(s_weatherTask);
    }

    if (_newsProvider) {
        s_newsTask = new NewsUpdateTask(_newsProvider, _logger);
        _scheduler->addTask(s_newsTask);
    }
}

void Application::_updateDeviceState() {
    _deviceState.uptimeMs = millis();

    if (_audioManager->isPlaying()) {
        _deviceState.status = DeviceStatus::PLAYING;
    } else if (_deviceState.status == DeviceStatus::PLAYING) {
        _deviceState.status = DeviceStatus::IDLE;
    }
}

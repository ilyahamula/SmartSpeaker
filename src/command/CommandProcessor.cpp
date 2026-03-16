#include "CommandProcessor.h"
#include "audio/AudioManager.h"
#include "content/IWeatherProvider.h"
#include "content/INewsProvider.h"
#include "content/IAssistantProvider.h"
#include "models/PlaybackRequest.h"
#include "Config.h"

static const char* TAG = "CmdProcessor";

CommandProcessor::CommandProcessor(ILogger* logger)
    : _logger(logger), _sourceCount(0)
{
    for (uint8_t i = 0; i < MAX_SOURCES; ++i) {
        _sources[i] = nullptr;
    }
}

void CommandProcessor::init() {
    LOG_INFO(_logger, TAG, "CommandProcessor initialized.");
}

void CommandProcessor::update() {
    for (uint8_t i = 0; i < _sourceCount; ++i) {
        ICommandSource* src = _sources[i];
        if (!src) continue;

        src->update();

        while (src->hasCommand()) {
            Command cmd = src->nextCommand();
            if (cmd.isValid()) {
                _dispatch(cmd);
            }
        }
    }
}

bool CommandProcessor::addCommandSource(ICommandSource* source) {
    if (!source || _sourceCount >= MAX_SOURCES) {
        LOG_WARN(_logger, TAG, "Cannot add command source — limit reached or null.");
        return false;
    }
    _sources[_sourceCount++] = source;
    LOG_INFO(_logger, TAG, source->getName());
    return true;
}

void CommandProcessor::setAudioManager(AudioManager* manager) {
    _audioManager = manager;
}

void CommandProcessor::setWeatherProvider(IWeatherProvider* provider) {
    _weatherProvider = provider;
}

void CommandProcessor::setNewsProvider(INewsProvider* provider) {
    _newsProvider = provider;
}

void CommandProcessor::setAssistantProvider(IAssistantProvider* provider) {
    _assistantProvider = provider;
}

// ---------------------------------------------------------------------------
// Private dispatch
// ---------------------------------------------------------------------------

void CommandProcessor::_dispatch(const Command& cmd) {
    LOG_INFO(_logger, TAG, cmd.rawText.c_str());

    switch (cmd.type) {
        case CommandType::PLAY_MUSIC:
            _handlePlayMusic(cmd);
            break;

        case CommandType::PAUSE_PLAYBACK:
        case CommandType::RESUME_PLAYBACK:
        case CommandType::STOP_PLAYBACK:
        case CommandType::NEXT_TRACK:
        case CommandType::PREV_TRACK:
        case CommandType::VOLUME_UP:
        case CommandType::VOLUME_DOWN:
        case CommandType::SET_VOLUME:
        case CommandType::MUTE:
        case CommandType::UNMUTE:
            _handleVolumeChange(cmd);
            break;

        case CommandType::GET_WEATHER:
            _handleWeatherRequest(cmd);
            break;

        case CommandType::GET_NEWS:
            _handleNewsRequest(cmd);
            break;

        case CommandType::ASK_QUESTION:
            _handleQuestion(cmd);
            break;

        case CommandType::SYSTEM_STATUS:
        case CommandType::REBOOT:
            _handleSystemCommand(cmd);
            break;

        default:
            LOG_WARN(_logger, TAG, "Unknown command type — ignoring.");
            break;
    }
}

void CommandProcessor::_handlePlayMusic(const Command& cmd) {
    if (!_audioManager) {
        LOG_WARN(_logger, TAG, "No AudioManager — cannot play music.");
        return;
    }
    PlaybackRequest req;
    req.query      = cmd.parameter.isEmpty() ? cmd.rawText : cmd.parameter;
    req.sourceType = PlaybackSourceType::STREAMING_URL; // Default; override once resolved
    _audioManager->play(req);
}

void CommandProcessor::_handleVolumeChange(const Command& cmd) {
    if (!_audioManager) return;

    switch (cmd.type) {
        case CommandType::PAUSE_PLAYBACK:  _audioManager->pause();   break;
        case CommandType::RESUME_PLAYBACK: _audioManager->resume();  break;
        case CommandType::STOP_PLAYBACK:   _audioManager->stop();    break;
        case CommandType::NEXT_TRACK:      _audioManager->next();    break;
        case CommandType::PREV_TRACK:      _audioManager->previous(); break;
        case CommandType::VOLUME_UP:       _audioManager->setVolume(_audioManager->getVolume() + 10); break;
        case CommandType::VOLUME_DOWN:     _audioManager->setVolume(_audioManager->getVolume() - 10); break;
        case CommandType::SET_VOLUME: {
            uint8_t vol = (uint8_t)cmd.parameter.toInt();
            _audioManager->setVolume(vol);
            break;
        }
        case CommandType::MUTE:   _audioManager->setVolume(0);  break;
        case CommandType::UNMUTE: _audioManager->setVolume(50); break;
        default: break;
    }
}

void CommandProcessor::_handleWeatherRequest(const Command& cmd) {
    if (!_weatherProvider) {
        LOG_WARN(_logger, TAG, "No WeatherProvider configured.");
        return;
    }
    const char* location = cmd.parameter.isEmpty()
                               ? WEATHER_DEFAULT_LOCATION
                               : cmd.parameter.c_str();
    _weatherProvider->fetchWeather(location);
    // TODO: when fetch completes, deliver WeatherData as audio via TextToSpeech or display
}

void CommandProcessor::_handleNewsRequest(const Command& cmd) {
    if (!_newsProvider) {
        LOG_WARN(_logger, TAG, "No NewsProvider configured.");
        return;
    }
    _newsProvider->fetchNews(NEWS_DEFAULT_MAX_ITEMS);
    // TODO: iterate items and deliver via TTS or display
}

void CommandProcessor::_handleQuestion(const Command& cmd) {
    if (!_assistantProvider) {
        LOG_WARN(_logger, TAG, "No AssistantProvider configured.");
        return;
    }
    _assistantProvider->sendQuery(cmd.rawText.c_str());
    // TODO: poll hasResponse() and deliver via TTS
}

void CommandProcessor::_handleSystemCommand(const Command& cmd) {
    switch (cmd.type) {
        case CommandType::SYSTEM_STATUS:
            LOG_INFO(_logger, TAG, "Status requested — TODO: broadcast DeviceState.");
            break;
        case CommandType::REBOOT:
            LOG_WARN(_logger, TAG, "Reboot requested.");
            ESP.restart();
            break;
        default:
            break;
    }
}

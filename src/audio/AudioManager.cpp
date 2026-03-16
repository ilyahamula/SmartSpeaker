#include "AudioManager.h"
#include <algorithm>

static const char* TAG = "AudioManager";

AudioManager::AudioManager(ILogger* logger)
    : _logger(logger), _playerCount(0), _activePlayer(nullptr), _volume(50)
{
    for (uint8_t i = 0; i < MAX_PLAYERS; ++i) {
        _players[i] = nullptr;
    }
}

bool AudioManager::init() {
    LOG_INFO(_logger, TAG, "Initializing AudioManager...");
    bool allOk = true;
    for (uint8_t i = 0; i < _playerCount; ++i) {
        if (_players[i] && !_players[i]->init()) {
            LOG_ERROR(_logger, TAG, _players[i]->getName());
            allOk = false;
        }
    }
    LOG_INFO(_logger, TAG, "AudioManager ready.");
    return allOk;
}

void AudioManager::update() {
    if (_activePlayer) {
        _activePlayer->update();
    }
}

bool AudioManager::addPlayer(IAudioPlayer* player) {
    if (!player || _playerCount >= MAX_PLAYERS) {
        LOG_WARN(_logger, TAG, "Cannot add player.");
        return false;
    }
    _players[_playerCount++] = player;
    LOG_INFO(_logger, TAG, player->getName());
    return true;
}

bool AudioManager::play(const PlaybackRequest& request) {
    IAudioPlayer* player = _selectPlayer(request);
    if (!player) {
        LOG_ERROR(_logger, TAG, "No player found for request.");
        return false;
    }

    // Stop currently active player before switching
    if (_activePlayer && _activePlayer != player) {
        _activePlayer->stop();
    }

    _activePlayer = player;
    _activePlayer->setVolume(_volume);
    return _activePlayer->play(request);
}

bool AudioManager::pause() {
    if (!_activePlayer) return false;
    return _activePlayer->pause();
}

bool AudioManager::resume() {
    if (!_activePlayer) return false;
    return _activePlayer->resume();
}

bool AudioManager::stop() {
    if (!_activePlayer) return false;
    bool ok = _activePlayer->stop();
    _activePlayer = nullptr;
    return ok;
}

bool AudioManager::next() {
    if (!_activePlayer) return false;
    return _activePlayer->next();
}

bool AudioManager::previous() {
    if (!_activePlayer) return false;
    return _activePlayer->previous();
}

bool AudioManager::setVolume(uint8_t volume) {
    _volume = (volume > 100) ? 100 : volume;
    if (_activePlayer) {
        return _activePlayer->setVolume(_volume);
    }
    return true;
}

uint8_t AudioManager::getVolume() const {
    return _volume;
}

PlaybackState AudioManager::getState() const {
    if (_activePlayer) {
        return _activePlayer->getState();
    }
    PlaybackState idle;
    idle.volume = _volume;
    return idle;
}

bool AudioManager::isPlaying() const {
    return _activePlayer && _activePlayer->getState().isPlaying();
}

IAudioPlayer* AudioManager::_selectPlayer(const PlaybackRequest& request) {
    for (uint8_t i = 0; i < _playerCount; ++i) {
        if (_players[i] && _players[i]->canHandle(request)) {
            return _players[i];
        }
    }
    return nullptr;
}

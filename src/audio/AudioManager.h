#pragma once
#include <Arduino.h>
#include "IAudioPlayer.h"
#include "models/PlaybackRequest.h"
#include "models/PlaybackState.h"
#include "logging/ILogger.h"

// -----------------------------------------------------------------------
// AudioManager — concrete coordinator that owns the audio player pool
// and routes PlaybackRequests to the appropriate IAudioPlayer.
//
// Players are injected via addPlayer().
// Routing logic: the first registered player that returns canHandle() == true
// is selected for playback.
// -----------------------------------------------------------------------
class AudioManager {
public:
    explicit AudioManager(ILogger* logger);
    ~AudioManager() = default;

    bool init();
    void update();  // Call from main loop — delegates to active player

    // Register a player implementation (up to MAX_PLAYERS)
    bool addPlayer(IAudioPlayer* player);

    // Playback control — delegates to active / selected player
    bool play(const PlaybackRequest& request);
    bool pause();
    bool resume();
    bool stop();
    bool next();
    bool previous();

    bool    setVolume(uint8_t volume);
    uint8_t getVolume() const;

    PlaybackState getState() const;

    bool isPlaying() const;

private:
    IAudioPlayer* _selectPlayer(const PlaybackRequest& request);

    static constexpr uint8_t MAX_PLAYERS = 4;

    IAudioPlayer*  _players[MAX_PLAYERS];
    uint8_t        _playerCount  = 0;

    IAudioPlayer*  _activePlayer = nullptr;   // Currently playing player
    uint8_t        _volume       = 50;        // 0–100 master volume

    ILogger*       _logger;
};

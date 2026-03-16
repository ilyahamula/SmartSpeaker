#pragma once
#include <Arduino.h>

enum class PlaybackStatus : uint8_t {
    STOPPED,
    PLAYING,
    PAUSED,
    BUFFERING,
    ERROR
};

// -----------------------------------------------------------------------
// PlaybackState — current state of the audio playback subsystem
// -----------------------------------------------------------------------
struct PlaybackState {
    PlaybackStatus status       = PlaybackStatus::STOPPED;

    String  currentTrack;     // Track title
    String  currentArtist;    // Artist name
    String  albumName;

    uint32_t positionMs        = 0;   // Playback position
    uint32_t durationMs        = 0;   // Track duration (0 if unknown)

    uint8_t  volume            = 0;
    bool     isMuted           = false;
    bool     isShuffle         = false;
    bool     isRepeat          = false;

    bool isPlaying() const { return status == PlaybackStatus::PLAYING; }
    bool isStopped() const { return status == PlaybackStatus::STOPPED; }
};

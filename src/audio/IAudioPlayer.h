#pragma once
#include <Arduino.h>
#include "models/PlaybackRequest.h"
#include "models/PlaybackState.h"

// -----------------------------------------------------------------------
// IAudioPlayer — abstract audio playback driver interface
//
// Concrete implementations (to be added):
//   - HttpStreamPlayer  (Audio.h or ESP8266Audio for HTTP/HTTPS streams)
//   - SpotifyPlayer     (Spotify Connect or spotify-esp32 library)
//   - LocalFlashPlayer  (LittleFS / SPIFFS + audio decoder)
//   - LocalSdPlayer     (SD card + audio decoder)
//   - BluetoothA2dpSink (receive audio over BT from phone)
// -----------------------------------------------------------------------
class IAudioPlayer {
public:
    virtual ~IAudioPlayer() = default;

    virtual bool init() = 0;

    virtual bool play(const PlaybackRequest& request) = 0;
    virtual bool pause()    = 0;
    virtual bool resume()   = 0;
    virtual bool stop()     = 0;
    virtual bool next()     = 0;
    virtual bool previous() = 0;

    virtual bool    setVolume(uint8_t volume) = 0;
    virtual uint8_t getVolume() const = 0;

    // Whether this player can handle the given request
    virtual bool canHandle(const PlaybackRequest& request) const = 0;

    // Current playback state snapshot
    virtual PlaybackState getState() const = 0;

    // Must be called from main loop — drives buffering, decode, etc.
    virtual void update() = 0;

    // Descriptive name for logging
    virtual const char* getName() const = 0;
};

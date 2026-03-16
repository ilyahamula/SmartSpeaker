#pragma once
#include <Arduino.h>

// Where audio content should come from
enum class PlaybackSourceType : uint8_t {
    STREAMING_SPOTIFY,
    STREAMING_YOUTUBE_MUSIC,
    STREAMING_URL,          // Direct HTTP/HTTPS audio stream
    LOCAL_FLASH,            // SPIFFS / LittleFS on-device storage
    LOCAL_SD_CARD           // SD card storage
};

// -----------------------------------------------------------------------
// PlaybackRequest — describes what should be played and from where
// -----------------------------------------------------------------------
struct PlaybackRequest {
    PlaybackSourceType sourceType = PlaybackSourceType::STREAMING_URL;

    String query;     // Human-readable search query ("play Bohemian Rhapsody")
    String uri;       // Resolved URI / track ID (filled after lookup, optional at request time)

    uint8_t volume    = 0;   // 0 = keep current volume; 1–100 = set volume
    bool    shuffle   = false;
    bool    repeat    = false;
};

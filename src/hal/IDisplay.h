#pragma once
#include <Arduino.h>
#include "models/PlaybackState.h"
#include "models/ConnectivityState.h"

// -----------------------------------------------------------------------
// IDisplay — hardware abstraction for a status display
//
// Concrete implementations: OledDisplay (SSD1306), TftDisplay (ILI9341),
// future: EpaperDisplay, NullDisplay (headless), etc.
// -----------------------------------------------------------------------
class IDisplay {
public:
    virtual ~IDisplay() = default;

    virtual bool init()  = 0;

    virtual void clear() = 0;

    // Low-level text
    virtual void showText(const char* text, uint8_t x = 0, uint8_t y = 0) = 0;

    // Semantic screen updates
    virtual void showStatus(const char* statusLine) = 0;
    virtual void showPlaybackState(const PlaybackState& state) = 0;
    virtual void showConnectivityState(const ConnectivityState& state) = 0;
    virtual void showError(const char* errorMessage) = 0;

    // Must be called from main loop to push buffer to screen if needed
    virtual void update() = 0;
};

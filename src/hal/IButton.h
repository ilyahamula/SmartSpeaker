#pragma once
#include <Arduino.h>

// -----------------------------------------------------------------------
// IButton — hardware abstraction for a digital push button
//
// Concrete implementations: GpioButton (direct GPIO with debounce),
// future: CapacitiveButton, I2CExpanderButton, etc.
// -----------------------------------------------------------------------
class IButton {
public:
    virtual ~IButton() = default;

    virtual void init() = 0;

    // Must be called from the main loop — updates internal debounce state
    virtual void update() = 0;

    // True while button is physically held down
    virtual bool isPressed() const = 0;

    // True for exactly one update() cycle after press begins
    virtual bool wasJustPressed() const = 0;

    // True for exactly one update() cycle after release
    virtual bool wasJustReleased() const = 0;
};

#pragma once
#include <Arduino.h>

// -----------------------------------------------------------------------
// ILed — hardware abstraction for a single LED or LED indicator
//
// Concrete implementations: GpioLed (direct GPIO),
// future: NeoPixelLed, PwmLed, etc.
// -----------------------------------------------------------------------
class ILed {
public:
    virtual ~ILed() = default;

    virtual void init()  = 0;

    virtual void on()    = 0;
    virtual void off()   = 0;
    virtual void toggle() = 0;

    // Non-blocking blink: call update() each loop iteration
    virtual void blink(uint32_t intervalMs) = 0;

    virtual bool isOn() const = 0;

    // Must be called from the main loop to drive blink logic
    virtual void update() = 0;
};

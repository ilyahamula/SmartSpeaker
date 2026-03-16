#pragma once
#include <Arduino.h>

// Overall lifecycle/operational status of the device
enum class DeviceStatus : uint8_t {
    BOOTING,
    INITIALIZING,
    IDLE,
    LISTENING,    // Waiting for voice wake word / command
    PROCESSING,   // Processing a command
    PLAYING,      // Actively playing audio
    ERROR
};

// -----------------------------------------------------------------------
// DeviceState — top-level snapshot of the device's current state
// -----------------------------------------------------------------------
struct DeviceState {
    DeviceStatus status       = DeviceStatus::BOOTING;
    String       statusMessage;   // Human-readable description of current status

    uint8_t  volume           = 50;    // Master volume 0–100
    bool     isMuted          = false;
    uint32_t uptimeMs         = 0;     // Updated each loop iteration

    bool hasError() const { return status == DeviceStatus::ERROR; }
};

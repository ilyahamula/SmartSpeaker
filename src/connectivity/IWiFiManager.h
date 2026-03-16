#pragma once
#include <Arduino.h>
#include "models/ConnectivityState.h"

// -----------------------------------------------------------------------
// IWiFiManager — abstraction for Wi-Fi station (STA) mode management
//
// Concrete implementations: Esp32WiFiManager (using WiFi.h),
// future: SimWiFiManager (for testing without hardware), etc.
// -----------------------------------------------------------------------
class IWiFiManager {
public:
    virtual ~IWiFiManager() = default;

    virtual bool init() = 0;

    // Attempt to connect; returns true if connected within timeout
    virtual bool connect(const char* ssid, const char* password) = 0;

    virtual bool disconnect() = 0;
    virtual bool reconnect()  = 0;

    // Must be called from loop — handles reconnection logic
    virtual void update() = 0;

    virtual bool isConnected() const = 0;

    virtual String getLocalIP()  const = 0;
    virtual String getSSID()     const = 0;
    virtual int8_t getRSSI()     const = 0;

    virtual WiFiConnectionStatus getStatus() const = 0;
};

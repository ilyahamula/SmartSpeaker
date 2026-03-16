#pragma once
#include "IWiFiManager.h"
#include "IBluetoothManager.h"
#include "models/ConnectivityState.h"
#include "logging/ILogger.h"

// -----------------------------------------------------------------------
// IConnectivityManager — top-level abstraction for all connectivity
//
// Owns and coordinates IWiFiManager and IBluetoothManager.
// Provides a unified state snapshot and connection lifecycle methods.
//
// Concrete implementation: Esp32ConnectivityManager (using WiFi.h + BT)
// -----------------------------------------------------------------------
class IConnectivityManager {
public:
    virtual ~IConnectivityManager() = default;

    virtual bool init() = 0;
    virtual void update() = 0;

    // Wi-Fi control
    virtual bool connectWiFi(const char* ssid, const char* password) = 0;
    virtual bool disconnectWiFi() = 0;
    virtual bool isWiFiConnected() const = 0;

    // Bluetooth control
    virtual bool enableBluetooth()  = 0;
    virtual bool disableBluetooth() = 0;
    virtual bool isBluetoothConnected() const = 0;

    // Unified state snapshot
    virtual ConnectivityState getState() const = 0;

    // True if any network route to internet is available
    virtual bool isNetworkAvailable() const = 0;
};

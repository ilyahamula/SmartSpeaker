#pragma once
#include <Arduino.h>
#include "models/ConnectivityState.h"

// -----------------------------------------------------------------------
// IBluetoothManager — abstraction for Bluetooth / BLE management
//
// Concrete implementations: Esp32ClassicBtManager (BluetoothA2DP),
//                           Esp32BleManager (NimBLE), etc.
// -----------------------------------------------------------------------
class IBluetoothManager {
public:
    virtual ~IBluetoothManager() = default;

    virtual bool init(const char* deviceName) = 0;

    virtual bool enable()  = 0;
    virtual bool disable() = 0;

    virtual bool startPairing() = 0;
    virtual bool stopPairing()  = 0;

    // Must be called from loop
    virtual void update() = 0;

    virtual bool isEnabled()   const = 0;
    virtual bool isConnected() const = 0;

    virtual String getPairedDeviceName() const = 0;

    virtual BluetoothStatus getStatus() const = 0;
};

#pragma once
#include <Arduino.h>

enum class WiFiConnectionStatus : uint8_t {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    CONNECTION_FAILED,
    RECONNECTING
};

enum class BluetoothStatus : uint8_t {
    DISABLED,
    IDLE,          // Enabled but not paired
    PAIRING,       // In pairing mode
    CONNECTED      // Paired and connected to a device
};

// -----------------------------------------------------------------------
// ConnectivityState — snapshot of all connectivity subsystems
// -----------------------------------------------------------------------
struct ConnectivityState {
    WiFiConnectionStatus wifiStatus       = WiFiConnectionStatus::DISCONNECTED;
    String               wifiSSID;
    int8_t               wifiRSSI         = 0;   // Signal strength in dBm
    String               localIP;

    BluetoothStatus      bluetoothStatus  = BluetoothStatus::DISABLED;
    String               bluetoothPairedDevice;

    bool isNetworkAvailable() const {
        return wifiStatus == WiFiConnectionStatus::CONNECTED;
    }
};

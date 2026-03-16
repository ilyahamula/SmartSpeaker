#pragma once

// =====================================================================
// HARDWARE PIN DEFINITIONS
// =====================================================================

// I2S Audio Output (e.g. MAX98357A amplifier)
#define PIN_I2S_BCLK            26
#define PIN_I2S_LRCK            25
#define PIN_I2S_DATA_OUT        22

// I2S Microphone Input (e.g. INMP441 MEMS mic)
#define PIN_I2S_MIC_BCLK        32
#define PIN_I2S_MIC_WS          33
#define PIN_I2S_MIC_DATA        34

// Control Buttons (active-low, internal pull-up)
#define PIN_BTN_PLAY_PAUSE      15
#define PIN_BTN_VOLUME_UP        4
#define PIN_BTN_VOLUME_DOWN      5
#define PIN_BTN_NEXT            18
#define PIN_BTN_PREV            19

// Status LEDs (active-high)
#define PIN_LED_STATUS           2   // General status / heartbeat
#define PIN_LED_WIFI            21   // Wi-Fi connected indicator
#define PIN_LED_BLUETOOTH       23   // Bluetooth active indicator

// SPI Display (e.g. SSD1306 OLED or ILI9341 TFT — optional)
#define PIN_DISPLAY_CS           5
#define PIN_DISPLAY_DC          27
#define PIN_DISPLAY_RST         14
#define PIN_SPI_MOSI            23
#define PIN_SPI_MISO            19
#define PIN_SPI_CLK             18

// SD Card (optional local storage)
#define PIN_SD_CS               13

// =====================================================================
// AUDIO CONFIGURATION
// =====================================================================
#define AUDIO_SAMPLE_RATE       44100
#define AUDIO_BITS_PER_SAMPLE      16
#define AUDIO_CHANNELS              2
#define AUDIO_DEFAULT_VOLUME       50   // 0–100

// =====================================================================
// APPLICATION CONFIGURATION
// =====================================================================
#define APP_SERIAL_BAUD_RATE   115200
#define APP_LOOP_DELAY_MS           5   // Main loop delay in ms

// =====================================================================
// CONNECTIVITY
// =====================================================================
#define WIFI_CONNECT_TIMEOUT_MS     10000UL
#define WIFI_RECONNECT_INTERVAL_MS  30000UL
#define BT_DEVICE_NAME              "SmartSpeaker"
#define BT_AUDIO_PROFILE_NAME       "SmartSpeakerAudio"

// =====================================================================
// SCHEDULER TASK INTERVALS
// =====================================================================
#define SCHEDULER_WEATHER_UPDATE_MS      (10UL * 60UL * 1000UL)  // 10 min
#define SCHEDULER_NEWS_UPDATE_MS         (30UL * 60UL * 1000UL)  // 30 min
#define SCHEDULER_CONNECTIVITY_CHECK_MS  (15UL * 1000UL)         // 15 sec
#define SCHEDULER_COMMAND_POLL_MS        (500UL)                  // 500 ms
#define SCHEDULER_STATUS_HEARTBEAT_MS    (5UL  * 1000UL)         // 5 sec

// =====================================================================
// CONTENT PROVIDER DEFAULTS
// =====================================================================
#define WEATHER_DEFAULT_LOCATION    "auto"   // Auto-detect from IP
#define NEWS_DEFAULT_MAX_ITEMS           5

/**
 * SmartSpeaker — ESP32 firmware entry point
 *
 * Architecture overview:
 * ┌─────────────────────────────────────────────────────────────────┐
 * │  main.cpp  ──►  Application                                     │
 * │                     ├── IConnectivityManager  (Wi-Fi / BT)      │
 * │                     ├── CommandProcessor                        │
 * │                     │       └── ICommandSource[]  (injected)    │
 * │                     ├── AudioManager                            │
 * │                     │       └── IAudioPlayer[]   (injected)     │
 * │                     ├── IWeatherProvider        (injected)      │
 * │                     ├── INewsProvider            (injected)     │
 * │                     ├── IAssistantProvider       (injected)     │
 * │                     ├── IDisplay                 (injected)     │
 * │                     └── SimpleScheduler                         │
 * └─────────────────────────────────────────────────────────────────┘
 *
 * To add a concrete integration:
 *   1. Create a class that implements the relevant interface.
 *   2. #include it here (or in a dedicated wiring header).
 *   3. Instantiate it and inject it via app.set*() / app.add*().
 *   4. No changes to Application or any other architecture file needed.
 *
 * Example (commented out — uncomment when real implementations exist):
 *
 *   #include "concrete/Esp32WiFiConnectivityManager.h"
 *   #include "concrete/TelegramBotCommandSource.h"
 *   #include "concrete/HttpStreamPlayer.h"
 *   #include "concrete/OpenWeatherMapProvider.h"
 *   #include "concrete/NewsApiProvider.h"
 *   #include "concrete/OpenAiAssistantProvider.h"
 *   #include "concrete/OledDisplay.h"
 */

#include <Arduino.h>
#include "app/Application.h"

// Application instance — heap-allocated so construction is deferred to setup()
static Application* app = nullptr;

void setup() {
    app = new Application();

    // ----------------------------------------------------------------
    // Inject concrete implementations here.
    // Until real implementations exist, the Application runs with
    // a SerialLogger, SimpleScheduler, and empty handlers — safe to
    // compile and test the architectural skeleton as-is.
    // ----------------------------------------------------------------

    // Example Wi-Fi connectivity (uncomment with real implementation):
    // app->setConnectivityManager(new Esp32WiFiConnectivityManager(
    //     WIFI_SSID, WIFI_PASSWORD, app->getLogger()
    // ));

    // Example command sources (uncomment with real implementations):
    // app->addCommandSource(new TelegramBotCommandSource(TELEGRAM_BOT_TOKEN, app->getLogger()));
    // app->addCommandSource(new VoiceCommandSource(PIN_I2S_MIC_DATA, app->getLogger()));

    // Example audio players (uncomment with real implementations):
    // app->addAudioPlayer(new HttpStreamPlayer(app->getLogger()));
    // app->addAudioPlayer(new LocalFlashPlayer(app->getLogger()));

    // Example content providers (uncomment with real implementations):
    // app->setWeatherProvider(new OpenWeatherMapProvider(OWM_API_KEY, app->getLogger()));
    // app->setNewsProvider(new NewsApiProvider(NEWS_API_KEY, app->getLogger()));
    // app->setAssistantProvider(new OpenAiAssistantProvider(OPENAI_API_KEY, app->getLogger()));

    // Example display (uncomment with real implementation):
    // app->setDisplay(new OledDisplay(PIN_DISPLAY_DC, PIN_DISPLAY_RST, app->getLogger()));

    app->setup();
}

void loop() {
    if (app) {
        app->loop();
    }
}

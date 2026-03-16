# SmartSpeaker — Architecture Notes

## Project structure

```
SmartSpeaker/
├── include/Config.h           ← all pins, constants, intervals
├── platformio.ini             ← updated with -I src, -std=c++17
└── src/
    ├── main.cpp               ← clean wiring entry point
    ├── app/                   ← Application orchestrator
    ├── logging/               ← ILogger + SerialLogger
    ├── models/                ← 9 data model / DTO headers
    ├── hal/                   ← ILed, IButton, IDisplay
    ├── connectivity/          ← IWiFiManager, IBluetoothManager, IConnectivityManager
    ├── command/               ← ICommandSource + CommandProcessor
    ├── audio/                 ← IAudioPlayer + AudioManager (player pool)
    ├── content/               ← IWeatherProvider, INewsProvider, IAssistantProvider
    └── scheduler/             ← IScheduler + SimpleScheduler (millis()-based)
```

## How the extension model works

Adding any concrete integration is a 3-step pattern — zero architecture changes required:

1. Create `src/concrete/YourImpl.h/.cpp` implementing the matching interface
2. `#include` it in `main.cpp`
3. Inject it:
   ```cpp
   app->addCommandSource(new TelegramBotCommandSource(...));
   ```

Every injection point is already stubbed with commented examples in `src/main.cpp`.

## Key design decisions

- **`Application`** owns infrastructure (`SerialLogger`, `SimpleScheduler`, `CommandProcessor`, `AudioManager`) by pointer and takes all pluggable dependencies via DI setters before `setup()`

- **`CommandProcessor`** polls all `ICommandSource` channels each loop and routes by `CommandType` to `AudioManager` / content providers — no callbacks needed

- **`AudioManager`** selects the right `IAudioPlayer` via `canHandle()` — adding Spotify vs. local flash vs. HTTP stream is just registering another player

- **`SimpleScheduler`** drives periodic tasks (heartbeat, connectivity check, weather/news refresh) without blocking the loop

- **`Config.h`** centralises all pin macros and tunable constants — one place to change board wiring

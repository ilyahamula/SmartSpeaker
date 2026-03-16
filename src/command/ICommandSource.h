#pragma once
#include <Arduino.h>
#include "models/Command.h"

// -----------------------------------------------------------------------
// ICommandSource — abstraction for any input channel that delivers commands
//
// Concrete implementations (to be added):
//   - GoogleAssistantSource (microphone + STT + NLU)
//   - TelegramBotSource    (Telegram Bot API polling)
//   - DiscordBotSource     (Discord Bot WebSocket/REST)
//   - ButtonSource         (physical button input)
// -----------------------------------------------------------------------
class ICommandSource {
public:
    virtual ~ICommandSource() = default;

    // Called once at startup
    virtual bool init() = 0;

    // Must be called from main loop — polls the source for new input
    virtual void update() = 0;

    // Returns true if at least one unprocessed command is available
    virtual bool hasCommand() const = 0;

    // Returns and removes the next command from the internal queue.
    // Only call if hasCommand() == true.
    virtual Command nextCommand() = 0;

    // Human-readable name for logging (e.g. "TelegramBot", "Voice")
    virtual const char* getName() const = 0;

    // Whether this source requires network connectivity
    virtual bool requiresNetwork() const = 0;
};

#pragma once
#include <Arduino.h>

// Source that generated the command
enum class CommandSource : uint8_t {
    VOICE_ASSISTANT,   // Google Assistant / on-device wake word
    TELEGRAM_BOT,      // Telegram Bot API message
    DISCORD_BOT,       // Discord Bot message
    BUTTON,            // Physical button press
    INTERNAL           // Internally generated (scheduler, self-test)
};

// High-level intent of the command
enum class CommandType : uint8_t {
    // Playback
    PLAY_MUSIC,
    PAUSE_PLAYBACK,
    RESUME_PLAYBACK,
    STOP_PLAYBACK,
    NEXT_TRACK,
    PREV_TRACK,
    // Volume
    VOLUME_UP,
    VOLUME_DOWN,
    SET_VOLUME,
    MUTE,
    UNMUTE,
    // Content
    GET_WEATHER,
    GET_NEWS,
    ASK_QUESTION,
    // System
    SYSTEM_STATUS,
    REBOOT,
    // Fallback
    UNKNOWN
};

// -----------------------------------------------------------------------
// Command — represents a single user or system command
// -----------------------------------------------------------------------
struct Command {
    CommandType   type      = CommandType::UNKNOWN;
    CommandSource source    = CommandSource::INTERNAL;

    String rawText;       // Original text / speech transcript
    String parameter;     // Optional: song name, city, question, volume level, etc.

    uint32_t timestampMs  = 0;  // millis() when command was received

    bool isValid() const { return type != CommandType::UNKNOWN; }
};

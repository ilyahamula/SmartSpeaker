#pragma once
#include <Arduino.h>

// -----------------------------------------------------------------------
// AssistantResponse — answer/response from an AI assistant provider
// -----------------------------------------------------------------------
struct AssistantResponse {
    String   text;          // Text of the response
    String   audioUrl;      // Optional URL to pre-synthesised audio
    bool     hasAudio;      // True if audioUrl is populated
    bool     success;       // False on error
    String   errorMessage;  // Populated on failure
};

#pragma once
#include <Arduino.h>
#include "models/AssistantResponse.h"

// -----------------------------------------------------------------------
// IAssistantProvider — abstraction for AI assistant / question-answering
//
// Concrete implementations (to be added):
//   - GoogleAssistantProvider  (Google Assistant SDK / Actions on Google)
//   - OpenAiChatProvider       (OpenAI ChatCompletion API)
//   - GeminiProvider           (Google Gemini API)
// -----------------------------------------------------------------------
class IAssistantProvider {
public:
    virtual ~IAssistantProvider() = default;

    virtual bool init() = 0;

    // Send a text query to the assistant. May be asynchronous.
    virtual bool sendQuery(const char* query) = 0;

    // True if a response is ready to be retrieved
    virtual bool hasResponse() const = 0;

    // Retrieve and consume the latest response.
    // Only call if hasResponse() == true.
    virtual AssistantResponse getResponse() = 0;

    virtual const char* getName() const = 0;
};

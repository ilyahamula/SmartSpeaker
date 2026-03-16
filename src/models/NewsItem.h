#pragma once
#include <Arduino.h>

// -----------------------------------------------------------------------
// NewsItem — a single news article headline/summary
// -----------------------------------------------------------------------
struct NewsItem {
    String title;
    String summary;
    String source;       // News outlet name
    String url;
    uint32_t publishedAt = 0;  // Unix timestamp (0 if unknown)
};

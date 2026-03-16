#pragma once
#include <Arduino.h>
#include "models/NewsItem.h"

// -----------------------------------------------------------------------
// INewsProvider — abstraction for news headline retrieval
//
// Concrete implementations (to be added):
//   - NewsApiProvider     (newsapi.org)
//   - RssFeedProvider     (any RSS 2.0 feed over HTTP)
// -----------------------------------------------------------------------
class INewsProvider {
public:
    virtual ~INewsProvider() = default;

    virtual bool init() = 0;

    // Fetch up to maxItems headlines. May be blocking.
    virtual bool fetchNews(uint8_t maxItems = 5) = 0;

    // Number of items available from the last successful fetch
    virtual uint8_t getItemCount() const = 0;

    // Return the item at index (0-based). Returns default NewsItem if out of range.
    virtual NewsItem getItem(uint8_t index) const = 0;

    virtual const char* getName() const = 0;
};

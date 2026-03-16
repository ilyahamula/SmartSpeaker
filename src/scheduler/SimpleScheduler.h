#pragma once
#include "IScheduler.h"
#include "logging/ILogger.h"

// -----------------------------------------------------------------------
// SimpleScheduler — millis()-based non-blocking periodic task scheduler
// Supports up to MAX_TASKS concurrent tasks.
// -----------------------------------------------------------------------
class SimpleScheduler : public IScheduler {
public:
    explicit SimpleScheduler(ILogger* logger);

    bool addTask(ISchedulerTask* task) override;
    void removeTask(const char* name) override;
    void update() override;

private:
    static constexpr uint8_t MAX_TASKS = 16;

    struct Entry {
        ISchedulerTask* task      = nullptr;
        uint32_t        lastRunMs = 0;
        bool            active    = false;
    };

    Entry    _entries[MAX_TASKS];
    uint8_t  _taskCount = 0;
    ILogger* _logger;
};

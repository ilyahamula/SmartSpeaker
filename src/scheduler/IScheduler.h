#pragma once
#include <Arduino.h>

// -----------------------------------------------------------------------
// ISchedulerTask — implement this to register a periodic task
// -----------------------------------------------------------------------
class ISchedulerTask {
public:
    virtual ~ISchedulerTask() = default;

    virtual const char* getName()        const = 0;
    virtual uint32_t    getIntervalMs()  const = 0;

    // Called by the scheduler when the interval has elapsed
    virtual void execute() = 0;
};

// -----------------------------------------------------------------------
// IScheduler — abstraction for a non-blocking periodic task runner
//
// Concrete implementations: SimpleScheduler (millis()-based),
// future: FreeRTOSScheduler (timer tasks), etc.
// -----------------------------------------------------------------------
class IScheduler {
public:
    virtual ~IScheduler() = default;

    // Register a periodic task. Does NOT take ownership.
    virtual bool addTask(ISchedulerTask* task) = 0;

    // Remove a task by name
    virtual void removeTask(const char* name) = 0;

    // Must be called from main loop — executes tasks whose interval has elapsed
    virtual void update() = 0;
};

#include "SimpleScheduler.h"
#include <string.h>

static const char* TAG = "Scheduler";

SimpleScheduler::SimpleScheduler(ILogger* logger) : _logger(logger), _taskCount(0) {
    for (uint8_t i = 0; i < MAX_TASKS; ++i) {
        _entries[i] = {};
    }
}

bool SimpleScheduler::addTask(ISchedulerTask* task) {
    if (!task || _taskCount >= MAX_TASKS) {
        LOG_WARN(_logger, TAG, "Cannot add task — limit reached or null.");
        return false;
    }
    Entry newEntry;
    newEntry.task = task;
    newEntry.lastRunMs = 0;
    newEntry.active = true;
    _entries[_taskCount++] = newEntry;
    LOG_INFO(_logger, TAG, task->getName());
    return true;
}

void SimpleScheduler::removeTask(const char* name) {
    for (uint8_t i = 0; i < _taskCount; ++i) {
        if (_entries[i].active &&
            _entries[i].task &&
            strcmp(_entries[i].task->getName(), name) == 0)
        {
            _entries[i].active = false;
            LOG_INFO(_logger, TAG, name);
            return;
        }
    }
}

void SimpleScheduler::update() {
    uint32_t now = millis();
    for (uint8_t i = 0; i < _taskCount; ++i) {
        Entry& e = _entries[i];
        if (!e.active || !e.task) continue;

        if (now - e.lastRunMs >= e.task->getIntervalMs()) {
            e.lastRunMs = now;
            e.task->execute();
        }
    }
}

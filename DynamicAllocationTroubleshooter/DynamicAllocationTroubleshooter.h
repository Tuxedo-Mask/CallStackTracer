#pragma once

#include <atomic>
#include <cstddef>
#include <iostream>
#include <map>
#include <mutex>

void* operator new(size_t requestedSize);
void operator delete(void* ptr) noexcept;

class DynamicAllocationTroubleshooter
{
public:
    static DynamicAllocationTroubleshooter& getInstance()
    {
        static DynamicAllocationTroubleshooter instance;
        return instance;
    }

    void enableMonitoring();
    void disableMonitoring();
    bool isMonitoringEnabled() const;

    void setFramesToSkip(size_t framesToSkip);
    size_t getFramesToSkip() const;

    void setFramesMaxDepth(size_t framesMaxDepth);
    size_t getFramesMaxDepth() const;

    size_t getStackTraceCacheSize() const;
    void addToStackTraceCache(const std::pair<const void*, std::string> traceRecord);
    void removeFromStackTraceCache(const void* traceKey);

    DynamicAllocationTroubleshooter(const DynamicAllocationTroubleshooter&) = delete;
    DynamicAllocationTroubleshooter& operator=(const DynamicAllocationTroubleshooter&) = delete;
public:
    enum class LogLevel {NoLogs, LogOnlyCounting, LogCallerLines, LogFullStackTrace, Debug};
    void setLogLevel(LogLevel logLevel);
    LogLevel getLogLevel() const;
private:
    DynamicAllocationTroubleshooter();
    ~DynamicAllocationTroubleshooter() = default;
    size_t m_framesToSkip;
    size_t m_framesMaxDepth;
    mutable std::mutex m_mutex;
    std::map<const void*, std::string> m_stackTraceCache;

    std::atomic<LogLevel> m_logLevel = LogLevel::NoLogs;
    std::atomic<bool> m_monitorAllocations = false;
};
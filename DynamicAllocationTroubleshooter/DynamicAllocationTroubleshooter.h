#pragma once

#include <atomic>
#include <cstddef>

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

    DynamicAllocationTroubleshooter(const DynamicAllocationTroubleshooter&) = delete;
    DynamicAllocationTroubleshooter& operator=(const DynamicAllocationTroubleshooter&) = delete;
private:
    DynamicAllocationTroubleshooter();
    ~DynamicAllocationTroubleshooter() = default;
    size_t m_framesToSkip;
    size_t m_framesMaxDepth;

    std::atomic<bool> monitorAllocations;
};
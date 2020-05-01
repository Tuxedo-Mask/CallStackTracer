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

    DynamicAllocationTroubleshooter(const DynamicAllocationTroubleshooter&) = delete;
    DynamicAllocationTroubleshooter& operator=(const DynamicAllocationTroubleshooter&) = delete;
private:
    DynamicAllocationTroubleshooter() = default;
    ~DynamicAllocationTroubleshooter() = default;

    std::atomic<bool> monitorAllocations;
};
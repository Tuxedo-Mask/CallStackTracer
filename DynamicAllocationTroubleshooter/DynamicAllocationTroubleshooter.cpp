#include "DynamicAllocationTroubleshooter.h"

#include <boost/stacktrace.hpp>
#include <boost/scope_exit.hpp>

#include <iostream>

namespace
{
    const size_t framesToSkipDefaultValue = 0;
    const size_t maxDepthDefaultValue = 100;
    std::atomic<bool> insideNewDelete = false;
}

DynamicAllocationTroubleshooter::DynamicAllocationTroubleshooter() : m_framesToSkip(framesToSkipDefaultValue), m_framesMaxDepth(maxDepthDefaultValue)
{
}

void DynamicAllocationTroubleshooter::setFramesToSkip(size_t framesToSkip)
{
    m_framesToSkip = framesToSkip;
}

size_t DynamicAllocationTroubleshooter::getFramesToSkip() const
{
    return m_framesToSkip;
}

void DynamicAllocationTroubleshooter::setFramesMaxDepth(size_t framesMaxDepth)
{
    m_framesMaxDepth = framesMaxDepth;
}

size_t DynamicAllocationTroubleshooter::getFramesMaxDepth() const
{
    return m_framesMaxDepth;
}

void DynamicAllocationTroubleshooter::enableMonitoring()
{
    monitorAllocations = true;
}

void DynamicAllocationTroubleshooter::disableMonitoring()
{
    monitorAllocations = false;
}

bool DynamicAllocationTroubleshooter::isMonitoringEnabled() const
{
    return monitorAllocations;
}

void* operator new(size_t requestedSize)
{
    void* allocationAddress = std::malloc(requestedSize);
    if (allocationAddress)
    {
        const auto& allocationTroubleshooter = DynamicAllocationTroubleshooter::getInstance();
        if (allocationTroubleshooter.isMonitoringEnabled() && !insideNewDelete)
        {
            insideNewDelete = true;
            BOOST_SCOPE_EXIT(&insideNewDelete) {
                insideNewDelete = false;
            } BOOST_SCOPE_EXIT_END
            std::cout << "Global operator new is called with requested size = " << requestedSize << std::endl;
            auto stackTrace = boost::stacktrace::stacktrace(allocationTroubleshooter.getFramesToSkip(), allocationTroubleshooter.getFramesMaxDepth());
            std::cout << stackTrace;
        }

        return allocationAddress;
    }
    else
    {
        throw std::bad_alloc{};
    }
}

void operator delete(void* ptr) noexcept
{
    const auto& allocationTroubleshooter = DynamicAllocationTroubleshooter::getInstance();
    if (allocationTroubleshooter.isMonitoringEnabled() && !insideNewDelete)
    {
        insideNewDelete = true;
        BOOST_SCOPE_EXIT(&insideNewDelete) {
            insideNewDelete = false;
        } BOOST_SCOPE_EXIT_END
        std::cout << "Global operator delete is called" << std::endl;
        
        auto stackTrace = boost::stacktrace::stacktrace(allocationTroubleshooter.getFramesToSkip(), allocationTroubleshooter.getFramesMaxDepth());
        std::cout << stackTrace;
    }
    std::free(ptr);
}
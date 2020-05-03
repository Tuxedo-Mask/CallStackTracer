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

size_t DynamicAllocationTroubleshooter::getStackTraceCacheSize() const
{
    std::lock_guard lck(m_mutex);
    return m_stackTraceCache.size();
}

void DynamicAllocationTroubleshooter::addToStackTraceCache(const std::pair<const void*, std::string> traceRecord)
{
    std::lock_guard lck(m_mutex);
    m_stackTraceCache.emplace(traceRecord);
}

void DynamicAllocationTroubleshooter::removeFromStackTraceCache(const void* traceKey)
{
    std::lock_guard lck(m_mutex);
    m_stackTraceCache.erase(traceKey);
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
        auto& allocationTroubleshooter = DynamicAllocationTroubleshooter::getInstance();
        if (allocationTroubleshooter.isMonitoringEnabled() && !insideNewDelete)
        {
            insideNewDelete = true;
            BOOST_SCOPE_EXIT(&insideNewDelete) {
                insideNewDelete = false;
            } BOOST_SCOPE_EXIT_END

            auto stackTrace = boost::stacktrace::stacktrace(allocationTroubleshooter.getFramesToSkip(), allocationTroubleshooter.getFramesMaxDepth());
            const auto& traceString = boost::stacktrace::detail::to_string(&stackTrace.as_vector()[0], stackTrace.size());
            allocationTroubleshooter.addToStackTraceCache(std::pair(allocationAddress, traceString));

            // HH:: TODO Add Logging flag and out stream
            // if ()
            // {
            //     std::cout << "Global operator new is called with requested size = " << requestedSize << std::endl;
            //     std::cout << traceString;
            // }
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
    auto& allocationTroubleshooter = DynamicAllocationTroubleshooter::getInstance();
    if (allocationTroubleshooter.isMonitoringEnabled() && !insideNewDelete)
    {
        insideNewDelete = true;
        BOOST_SCOPE_EXIT(&insideNewDelete) {
            insideNewDelete = false;
        } BOOST_SCOPE_EXIT_END
        
        auto stackTrace = boost::stacktrace::stacktrace(allocationTroubleshooter.getFramesToSkip(), allocationTroubleshooter.getFramesMaxDepth());
        const auto& traceString = boost::stacktrace::detail::to_string(&stackTrace.as_vector()[0], stackTrace.size());
        allocationTroubleshooter.removeFromStackTraceCache(ptr);
        // HH:: TODO Add Logging flag and out stream
        // if ()
        // {
        //     std::cout << "stackTraceCache.size() is " << allocationTroubleshooter.getStackTraceCacheSize() << std::endl;
        //     std::cout << "Global operator delete is called" << std::endl;        
        //     std::cout << traceString;
        // }
    }
    std::free(ptr);
}
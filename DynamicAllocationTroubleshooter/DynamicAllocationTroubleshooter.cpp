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
    m_monitorAllocations = true;
}

void DynamicAllocationTroubleshooter::disableMonitoring()
{
    m_monitorAllocations = false;
}

bool DynamicAllocationTroubleshooter::isMonitoringEnabled() const
{
    return m_monitorAllocations;
}

void DynamicAllocationTroubleshooter::setLogLevel(DynamicAllocationTroubleshooter::LogLevel logLevel)
{
    m_logLevel = logLevel;
}

DynamicAllocationTroubleshooter::LogLevel DynamicAllocationTroubleshooter::getLogLevel() const
{
    return m_logLevel;
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

            switch(allocationTroubleshooter.getLogLevel())
            {
                case DynamicAllocationTroubleshooter::LogLevel::Debug:
                {
                }
                break;
                case DynamicAllocationTroubleshooter::LogLevel::LogFullStackTrace:
                {
                }
                break;
                case DynamicAllocationTroubleshooter::LogLevel::LogCallerLines:
                {
                }
                break;
                case DynamicAllocationTroubleshooter::LogLevel::LogOnlyCounting:
                {
                    std::cout << "Operator new is called (+) " << std::endl;
                }
                break;                
                case DynamicAllocationTroubleshooter::LogLevel::NoLogs:
                {
                }
                break;
                default:
                {
                    assert(!"New Log level is added and is unhandled");
                }
                break;
            }
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
        
        switch(allocationTroubleshooter.getLogLevel())
        {
            case DynamicAllocationTroubleshooter::LogLevel::Debug:
            {
            }
            break;
            case DynamicAllocationTroubleshooter::LogLevel::LogFullStackTrace:
            {
            }
            break;
            case DynamicAllocationTroubleshooter::LogLevel::LogCallerLines:
            {
            }
            break;
            case DynamicAllocationTroubleshooter::LogLevel::LogOnlyCounting:
            {
                std::cout << "Operator delete is called (-)\nstackTraceCache.size() is "
                          << allocationTroubleshooter.getStackTraceCacheSize() << std::endl;
            }
            break;                
            case DynamicAllocationTroubleshooter::LogLevel::NoLogs:
            {
            }
            break;
            default:
            {
                assert(!"New Log level is added and is unhandled");
            }
            break;
        }
    }
    std::free(ptr);
}
#include "DynamicAllocationTroubleshooter.h"

#include <boost/stacktrace.hpp>
#include <boost/scope_exit.hpp>

#include <iostream>

namespace
{
    std::atomic<bool> insideNewDelete = false;
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
        if (DynamicAllocationTroubleshooter::getInstance().isMonitoringEnabled() && !insideNewDelete)
        {
            insideNewDelete = true;
            BOOST_SCOPE_EXIT(&insideNewDelete) {
                insideNewDelete = false;
            } BOOST_SCOPE_EXIT_END
            std::cout << "Global operator new is called with requested size = " << requestedSize << std::endl;
            //auto stackTrace = boost::stacktrace::basic_stacktrace<CustomAllocator<boost::stacktrace::frame>>();
            auto stackTrace = boost::stacktrace::stacktrace(1/*skip*/, 100/*max depth*/);
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
    if (DynamicAllocationTroubleshooter::getInstance().isMonitoringEnabled() && !insideNewDelete)
    {
        insideNewDelete = true;
        BOOST_SCOPE_EXIT(&insideNewDelete) {
            insideNewDelete = false;
        } BOOST_SCOPE_EXIT_END
        std::cout << "Global operator delete is called" << std::endl;
        
        auto stackTrace = boost::stacktrace::stacktrace(1/*skip*/, 100/*max depth*/);
        std::cout << stackTrace;
    }
    std::free(ptr);
}
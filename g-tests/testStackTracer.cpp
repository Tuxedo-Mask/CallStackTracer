#include <gtest/gtest.h>

#include "../DynamicAllocationTroubleshooter/DynamicAllocationTroubleshooter.h"

TEST(TestStackTracer, StartDisabledTest)
{
    EXPECT_FALSE(DynamicAllocationTroubleshooter::getInstance().isMonitoringEnabled());
}

TEST(TestStackTracer, EnableTest)
{
    auto& allocationTroubleshooter = DynamicAllocationTroubleshooter::getInstance();
    allocationTroubleshooter.enableMonitoring();
    EXPECT_TRUE(DynamicAllocationTroubleshooter::getInstance().isMonitoringEnabled());
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

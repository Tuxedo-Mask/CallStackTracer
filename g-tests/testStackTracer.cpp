#include <gtest/gtest.h>

#include "../DynamicAllocationTroubleshooter/DynamicAllocationTroubleshooter.h"

TEST(TestStackTracer, EnableTesting)
{
    EXPECT_FALSE(DynamicAllocationTroubleshooter::getInstance().isMonitoringEnabled());
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

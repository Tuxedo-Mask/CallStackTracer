#include <iostream>
#include <map>

#include "ContainersWithCustomAllocator/CustomAllocatedContainers.h"
#include "DynamicAllocationTroubleshooter/DynamicAllocationTroubleshooter.h"

int main()
{
    std::cout << "Welcome to the real world!" << std::endl;

    DynamicAllocationTroubleshooter::getInstance().enableMonitoring();
    int* testPtr = new int;
    
    delete testPtr;

    int* testArrPtr = new int[10];
    delete[] testArrPtr;
    // CustomAllocatedMap<int, int> vec;
    /*std::map<int, int> map;

    for (int i = 0; i < 10; ++i)
    {
        map.insert(std::make_pair(i, i + i));
    }*/

    return 0;
}

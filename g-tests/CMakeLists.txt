cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Either specify BOOST_ROOT or BOOST_INCLUDEDIR and BOOST_LIBRARYDIR.
# I am using "MinGW Distro" in this example, but also a relative path, e. g.
# "vendor/boost" can be specified.
#set (BOOST_ROOT "C:/MinGW")
set (BOOST_INCLUDEDIR "/usr/include/boost")
#set (BOOST_LIBRARYDIR "C:/MinGW/lib")
set (Boost_NO_SYSTEM_PATHS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS OFF)
set (Boost_USE_STATIC_RUNTIME OFF)
set (BOOST_ALL_DYN_LINK OFF)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests.exe testStackTracer.cpp ../DynamicAllocationTroubleshooter/DynamicAllocationTroubleshooter.cpp)
target_link_libraries(runTests.exe ${GTEST_LIBRARIES} dl pthread)

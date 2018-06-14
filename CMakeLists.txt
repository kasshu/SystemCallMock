cmake_minimum_required(VERSION 2.6)
 
#For gmock
set(GMOCK_ROOT "/usr/src/gmock")
set(CMAKE_MODULE_PATH "./CMakeModules")

# Locate GTest
find_package(GTest REQUIRED)
find_package(GMock REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Link runTests with what we want to test and the GTest and pthread library
add_definitions(-std=c++14 -ggdb)
include_directories(.)
add_executable(IOTest test/io_test.cpp src/io.cpp)
target_link_libraries(IOTest ${GTEST_LIBRARIES} ${GMOCK_LIBRARIES} ${GMOCK_MAIN_LIBRARIES} pthread)

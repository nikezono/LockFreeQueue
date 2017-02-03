cmake_minimum_required(VERSION 3.3)
project(LockfreeQueue)

enable_testing()
add_subdirectory(third_party/googletest)
include_directories(third_party/googletest/include)
link_directories(third_party/googletest/googlemock/gtest/)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -ggdb")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics --save-temps") # DEBUG

add_subdirectory(src)
add_subdirectory(test)


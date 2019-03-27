cmake_minimum_required(VERSION 2.8)
project(Game)

set(CMAKE_CXX_STANDARD 14)

if(DEBUG)
    add_subdirectory(googletest-master)
    include_directories(googletest-master/googletest/include)
    include_directories(googletest-master/googlemock/include)
endif()

include_directories(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if(DEBUG)
    include_directories(test)
    set(SOURCE_FILES test/tests.cpp)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG=1")
endif()

add_executable(game src/main.cpp src/GameSocket.cpp ${SOURCE_FILES})
if(DEBUG)
    target_link_libraries(game gtest gtest_main)
endif()
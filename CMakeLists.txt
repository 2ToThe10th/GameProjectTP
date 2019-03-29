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
    set(SOURCE_DEBUG_FILES test/tests.cpp)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG=1")
endif()

set(SOURCE_FILES src/main.cpp src/GameSocket.cpp src/UnitFactory.cpp src/UnitFactory.h src/CombatUnit.cpp src/CombatUnit.h src/Warrior.cpp src/Warrior.h src/Archer.cpp src/Archer.h src/Wizard.cpp src/Wizard.h src/City.cpp src/City.h src/Enums.h src/CityFactory.cpp src/CityFactory.h src/Unit.cpp src/Unit.h src/Location.cpp src/Location.h src/WaterUnitFactory.cpp src/WaterUnitFactory.h src/FireUnitFactory.cpp src/FireUnitFactory.h src/EarthUnitFactory.cpp src/EarthUnitFactory.h src/AirUnitFactory.cpp src/AirUnitFactory.h src/Map.cpp src/Map.h src/Colonist.cpp src/Colonist.h src/Worker.cpp src/Worker.h src/Money.cpp src/Money.h)

add_executable(game ${SOURCE_FILES} ${SOURCE_DEBUG_FILES})
if(DEBUG)
    target_link_libraries(game gtest gtest_main)
endif()
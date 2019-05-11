cmake_minimum_required(VERSION 2.8)
project(Game)

set(CMAKE_CXX_STANDARD 14)

if(DEBUG)
    add_subdirectory(googletest-master)
    include_directories(googletest-master/googletest/include)
    include_directories(googletest-master/googlemock/include)
endif()

include_directories(src)

if(DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")
    include_directories(test)
    set(SOURCE_DEBUG_FILES test/tests.cpp)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG=1")
endif()

set(SOURCE_FILES src/main.cpp src/GameSocket.cpp src/UnitFactory.cpp src/UnitFactory.h src/CombatUnit.cpp src/CombatUnit.h src/Warrior.cpp src/Warrior.h src/Archer.cpp src/Archer.h src/Wizard.cpp src/Wizard.h src/City.cpp src/City.h src/Enums.h src/CityFactory.cpp src/CityFactory.h src/Unit.cpp src/Unit.h src/Location.cpp src/Location.h src/Map.cpp src/Map.h src/Colonist.cpp src/Colonist.h src/Worker.cpp src/Worker.h src/Money.cpp src/Money.h src/Command/ICommand.cpp src/Command/ICommand.h src/Command/BuildCity.cpp src/Command/BuildCity.h src/Command/CreateUnit.cpp src/Command/CreateUnit.h src/Command/CreateTower.cpp src/Command/CreateTower.h src/Command/End.cpp src/Command/End.h src/Command/Mine.cpp src/Command/Mine.h src/Command/Go.cpp src/Command/Go.h src/Command/Attack.cpp src/Command/Attack.h)

add_executable(game ${SOURCE_FILES} ${SOURCE_DEBUG_FILES})
if(DEBUG)
    target_link_libraries(game gtest gtest_main)
endif()
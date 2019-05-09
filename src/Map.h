#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <string>
#include "Enums.h"
#include "Location.h"
#include "City.h"

class CombatUnit;
class Colonist;
class Worker;

class Map {
public:
    static const int MAX_HEIGHT = 20;
    static const int MAX_WIDTH = 20;
    static const int ALL = 100;
    static const int GOLD = 5;
    static const int SILVER = 10;
    static const int WOOD = 15;
private:
    CombatUnit* map_combat[MAX_HEIGHT][MAX_WIDTH];
    Colonist* map_colonist[MAX_HEIGHT][MAX_WIDTH];
    Worker* map_worker[MAX_HEIGHT][MAX_WIDTH];
    City* map_city[MAX_HEIGHT][MAX_WIDTH];
    Resource map_resource[MAX_HEIGHT][MAX_WIDTH];
public:

    Map();
    ~Map() = default;

    void Generate();
    std::string Info();

    CombatUnit*& combat(Location location);
    Colonist*& colonist(Location location);
    Worker*& worker(Location location);
    City*& city(Location location);
    Resource resource(Location location);
};


#endif //GAME_MAP_H

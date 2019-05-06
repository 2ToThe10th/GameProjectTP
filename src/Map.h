#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <string>
#include "CombatUnit.h"
#include "Colonist.h"
#include "Worker.h"
#include "City.h"

class Map {
private:
    static const int MAX_HEIGHT = 20;
    static const int MAX_WIDTH = 20;

    CombatUnit* map_combat[MAX_HEIGHT][MAX_WIDTH];
    Colonist* map_colonist[MAX_HEIGHT][MAX_WIDTH];
    Worker* map_worker[MAX_HEIGHT][MAX_WIDTH];
    City* map_city[MAX_HEIGHT][MAX_WIDTH];
public:

    Map();

    std::string Info();

    CombatUnit*& combat(Location location);
    Colonist*& colonist(Location location);
    Worker*& worker(Location location);
    City*& city(Location location);
};


#endif //GAME_MAP_H

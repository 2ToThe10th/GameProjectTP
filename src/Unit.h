#ifndef GAME_UNIT_H
#define GAME_UNIT_H

#include <string>
#include "Enums.h"
#include "Location.h"

class UnitFactory;
class Map;

class Unit {
protected:
    const int MAX_HEALTH;
    Location location;
    bool already_move = true;
    bool frozen = false;
    Map* map;
public:
    int health;
    Player which;
    unsigned int id;
    const Race race;
    static UnitFactory* my_unit_factory;
    static UnitFactory* opponent_unit_factory;
    explicit Unit(Player which, Race race, int health, Location city_location, Map *map, unsigned int id);
    virtual ~Unit() = default;

    virtual std::string Info();
    virtual void Go(Direction where) = 0;

    void NewTurn();
    Location GetLocation() const;
};


#endif //GAME_UNIT_H

#ifndef GAME_UNIT_H
#define GAME_UNIT_H

#include <string>
#include "Enums.h"
#include "Location.h"

class UnitFactory;
class Map;

class Unit {
protected:
    Location location;
    bool already_move = true;
    bool frozen = false;
    Map* map;
public:
    const int MAX_HEALTH;
    int health;
    Player which;
    unsigned int id;
    const Race race;
    static UnitFactory* my_unit_factory;
    static UnitFactory* opponent_unit_factory;
    explicit Unit(Player which, Race race, int health, Location city_location, Map *map, unsigned int id);
    virtual ~Unit() = default;

    virtual std::string Info();
    virtual unsigned int Go(Direction where) = 0;

    void NewTurn();
    void Froze();
    Location GetLocation() const;
};


#endif //GAME_UNIT_H

#ifndef GAME_UNIT_H
#define GAME_UNIT_H

#include "Enums.h"
#include "Location.h"

class Unit {
private:
    const int MAX_HEALTH;
    int health;
    Location location;
    bool already_move;
    bool frozen;
public:
    const Player which;
    const Race race;
    explicit Unit(Player which, Race race);
    virtual ~Unit();

    virtual void Info() = 0;
    virtual void Go(Direction where) = 0;

};


#endif //GAME_UNIT_H

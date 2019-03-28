#ifndef GAME_ARCHER_H
#define GAME_ARCHER_H

#include "CombatUnit.h"

class Archer: public CombatUnit {
    explicit CombatUnit(Player which, Race race);
    virtual ~CombatUnit();

    virtual void Go(Direction where) = 0;
    virtual void Attack(int x_to, int y_to) = 0;
};


#endif //GAME_ARCHER_H

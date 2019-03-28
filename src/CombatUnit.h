#ifndef GAME_COMBATUNIT_H
#define GAME_COMBATUNIT_H

#include "Enums.h"
#include "Unit.h"

class CombatUnit: public Unit {
private:
    int damage;
public:
    explicit CombatUnit(Player which, Race race);
    virtual ~CombatUnit();

    virtual void Go(Direction where) = 0;
    virtual void Attack(int x_to, int y_to) = 0;
};


#endif //GAME_COMBATUNIT_H

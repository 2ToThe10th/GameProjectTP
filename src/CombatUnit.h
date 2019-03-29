#ifndef GAME_COMBATUNIT_H
#define GAME_COMBATUNIT_H

#include "Enums.h"
#include "Unit.h"

class CombatUnit: public Unit {
private:
    int damage;
public:
    explicit CombatUnit(Player which, Race race, int health, Location city_location, Map& map, int damage);
    ~CombatUnit() override = default;

    std::string Info();

    //TODO:
    void Go(Direction where) override;
    //virtual void Attack(int x_to, int y_to) = 0;
};


#endif //GAME_COMBATUNIT_H

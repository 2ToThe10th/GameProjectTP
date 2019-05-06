#ifndef GAME_COMBATUNIT_H
#define GAME_COMBATUNIT_H

#include "Unit.h"

class CombatUnit: public Unit {
private:
    int damage;
public:
    explicit CombatUnit(Player which, Race race, int health, Location city_location, Map* map, int damage, unsigned int id);
    ~CombatUnit() override = default;

    virtual std::string Info();
    void Go(Direction where) override;
    virtual void Attack(Location where_attack) = 0;
};


#endif //GAME_COMBATUNIT_H

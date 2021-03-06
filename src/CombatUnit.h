#ifndef GAME_COMBATUNIT_H
#define GAME_COMBATUNIT_H

#include <vector>
#include "Unit.h"

class CombatUnit: public Unit {
public:
    const CombatUnitType combat_unit_type;
    int damage;
    explicit CombatUnit(Player which, Race race, int health, Location city_location, Map *map, int damage,
                        unsigned int id,
                        CombatUnitType combat_unit_type);
    ~CombatUnit() override;

    std::string Info() override ;
    unsigned int Go(Direction where) override;
    virtual unsigned int Attack(std::vector<Direction> &where_attack) = 0;
};


#endif //GAME_COMBATUNIT_H

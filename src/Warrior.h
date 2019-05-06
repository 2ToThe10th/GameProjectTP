#ifndef GAME_WARRIOR_H
#define GAME_WARRIOR_H

#include "CombatUnit.h"

class Warrior: public CombatUnit {
public:
    static const int cost_wood = 50;
    static const int cost_iron = 50;
    static const int cost_gold = 10;

    explicit Warrior(Player which, Race race, int health, Location city_location, Map* map, int damage, unsigned int id);

    ~Warrior() override = default;

    std::string Info() override;
    void Attack(Location where_attack) override {};
};


#endif //GAME_WARRIOR_H

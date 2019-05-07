#ifndef GAME_ARCHER_H
#define GAME_ARCHER_H

#include "CombatUnit.h"

class Archer: public CombatUnit {
public:
    static const int cost_wood = 100;
    static const int cost_iron = 50;
    static const int cost_gold = 20;

    explicit Archer(Player which, Race race, int health, Location city_location, Map* map, int damage, unsigned int id);
    ~Archer() override = default;

    std::string Info() override;
    void Attack(std::vector<Direction>& where_attack) override {};
};


#endif //GAME_ARCHER_H

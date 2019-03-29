#ifndef GAME_WARRIOR_H
#define GAME_WARRIOR_H

#include "CombatUnit.h"

class Warrior: public CombatUnit {
public:
    static const int cost_wood = 50;
    static const int cost_iron = 50;
    static const int cost_gold = 10;

    explicit Warrior(Player which, Race race, int health, Location city_location, Map& map, int damage);

    ~Warrior() override = default;

    std::string Info() override;

    //TODO:
    //virtual void Attack(int x_to, int y_to) = 0;
};


#endif //GAME_WARRIOR_H

#ifndef GAME_WIZARD_H
#define GAME_WIZARD_H

#include "CombatUnit.h"

class Wizard: public CombatUnit {
public:
    static const int cost_wood = 100;
    static const int cost_iron = 100;
    static const int cost_gold = 100;

    explicit Wizard(Player which, Race race, int health, Location city_location, Map* map, int damage);
    ~Wizard() override = default;

    std::string Info();

    //TODO:
    //virtual void Attack(int x_to, int y_to) = 0;
};


#endif //GAME_WIZARD_H

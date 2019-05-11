#ifndef GAME_WIZARD_H
#define GAME_WIZARD_H

#include "CombatUnit.h"

class Wizard: public CombatUnit {
public:
    static const int cost_wood = 100;
    static const int cost_iron = 100;
    static const int cost_gold = 100;

    explicit Wizard(Player which, Race race, int health, Location city_location, Map* map, int damage, unsigned int id);
    ~Wizard() override = default;

    std::string Info() override;
    unsigned int Attack(std::vector<Direction> &where_attack) override;
};


#endif //GAME_WIZARD_H

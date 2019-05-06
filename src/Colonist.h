#ifndef GAME_COLONIST_H
#define GAME_COLONIST_H

#include "Unit.h"

class Colonist: public Unit {
public:
    static const int cost_wood = 150;
    static const int cost_iron = 50;
    static const int cost_gold = 0;
    
    explicit Colonist(Player which, Race race, int health, Location city_location, Map* map, unsigned int id);
    ~Colonist() override = default;
    
    std::string Info() override;
    void Go(Direction where) override;
};


#endif //GAME_COLONIST_H

#ifndef GAME_WORKER_H
#define GAME_WORKER_H

#include "Enums.h"
#include "Unit.h"

class Worker: public Unit {
public:
    static const int cost_wood = 50;
    static const int cost_iron = 10;
    static const int cost_gold = 0;

    explicit Worker(Player which, Race race, int health, Location city_location, Map*& map);

    ~Worker() override = default;
    std::string Info() override;

    //TODO:
    //void Go(Direction where) override {}
};


#endif //GAME_WORKER_H

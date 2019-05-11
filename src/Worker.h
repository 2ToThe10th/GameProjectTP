#ifndef GAME_WORKER_H
#define GAME_WORKER_H

#include "Unit.h"
#include "Money.h"

class Worker: public Unit {
public:
    static const int cost_wood = 50;
    static const int cost_iron = 10;
    static const int cost_gold = 0;

    int cary_gold = 0;
    int cary_silver = 0;
    int cary_wood = 0;

    explicit Worker(Player which, Race race, int health, Location city_location, Map* map, unsigned int id);
    ~Worker() override;

    std::string Info() override;
    unsigned int Go(Direction where) override;
    unsigned int Mine();
};


#endif //GAME_WORKER_H

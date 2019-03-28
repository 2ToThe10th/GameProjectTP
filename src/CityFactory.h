#ifndef GAME_CITYFACTORY_H
#define GAME_CITYFACTORY_H

#include "Enums.h"
#include "City.h"
#include "Сolonist.h"
#include <vector>
#include <string>

const int START_RESOURCES_AMT = 100500;

class CityFactory {
private:
    Player which_city_factory;
    int gold_amt = START_RESOURCES_AMT;
    int iron_amt = START_RESOURCES_AMT;
    int wood_amt = START_RESOURCES_AMT;

public:
    std::vector<City> cities;

    CityFactory(Player which);

    void AddCity(Colonist colonist) {};

    std::string Info();
};


#endif //GAME_CITYFACTORY_H

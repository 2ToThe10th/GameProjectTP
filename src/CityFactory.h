#ifndef GAME_CITYFACTORY_H
#define GAME_CITYFACTORY_H

#include "Enums.h"
#include "City.h"
#include "Сolonist.h"
#include <vector>



class CityFactory {
private:
    Player which_city_factory;
public:
    int gold_amt;
    int iron_amt;
    int wood_amt;
    std::vector<City> cities;

    explicit CityFactory(Player which);

    void AddCity(Colonist colonist);
};


#endif //GAME_CITYFACTORY_H

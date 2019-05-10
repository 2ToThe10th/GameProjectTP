#ifndef GAME_CITYFACTORY_H
#define GAME_CITYFACTORY_H

#include <vector>
#include <string>
#include "Enums.h"
#include "City.h"
#include "Colonist.h"

class City;

class CityFactory {
private:
    Player which_city_factory;
    Map* map;

public:
    std::vector<City*> cities;

    CityFactory(Player which, Map* map);
    ~CityFactory();

    unsigned int AddCity(Colonist* colonist);

    std::string Info();
};


#endif //GAME_CITYFACTORY_H

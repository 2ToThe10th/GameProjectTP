#ifndef GAME_CITY_H
#define GAME_CITY_H

#include "Enums.h"
#include "Location.h"
#include "Money.h"
#include "CityFactory.h"

class CityFactory;

class City {
private:
    bool exist_archer_tower = false;
    bool exist_wizard_tower = false;
public:
    static CityFactory* my_city_factory;
    static CityFactory* opponent_city_factory;

    unsigned int id = -1;
    Player which;
    const Location location;
    explicit City(Location location);

    unsigned int BuildArcherTower(Money& money);
    unsigned int BuildWizardTower(Money& money);

    bool IsArcherTowerExist();
    bool IsWizardTowerExist();
};


#endif //GAME_CITY_H

#ifndef GAME_CITY_H
#define GAME_CITY_H

#include "Enums.h"
#include "Location.h"
#include "Money.h"

class City {
private:
    bool exist_archer_tower = false;
    bool exist_wizard_tower = false;
    bool something_is_building = false;
public:
    Location location;
    City(Location location);

    void BuildArcherTower(Money& money);
    void BuildWizardTower(Money& money);

    bool IsArcherTowerExist();
    bool IsWizardTowerExist();
};


#endif //GAME_CITY_H

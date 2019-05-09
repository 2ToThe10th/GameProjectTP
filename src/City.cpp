#include <iostream>
#include "City.h"

using std::cout;
using std::endl;

CityFactory* City::my_city_factory = nullptr;
CityFactory* City::opponent_city_factory = nullptr;

City::City(Location location): location(location) {}


void City::BuildArcherTower(Money& money) {
    if(exist_archer_tower) {
        cout << "Archer Town is already exist" << endl;
        return;
    }
    if(!money.Take(300, 100, 0)) {
        cout << "You don't have enough money" << endl;
        cout << money.Info() << endl;
        return;
    }

    exist_archer_tower = true;
}

void City::BuildWizardTower(Money& money) {
    if(!exist_archer_tower) {
        cout << "Archer Town isn't already exist. Build Archer Town to be able to Build Wizard Town" << endl;
        return;
    }
    if(exist_wizard_tower) {
        cout << "Wizard Town is already exist" << endl;
        return;
    }
    if(!money.Take(300, 300, 100)) {
        cout << "You don't have enough money" << endl;
        cout << money.Info() << endl;
        return;
    }

    exist_wizard_tower = true;
}

bool City::IsArcherTowerExist() {
    return exist_archer_tower;
}

bool City::IsWizardTowerExist() {
    return exist_wizard_tower;
}

#ifndef GAME_UNITFACTORY_H
#define GAME_UNITFACTORY_H

#include "CombatUnit.h"
#include "City.h"
#include "Enums.h"
#include "Colonist.h"
#include "Worker.h"
#include "Map.h"
#include "Money.h"
#include <iostream>
#include <string>
#include <vector>

class UnitFactory {
protected:
    Player which;
    Map* map;
    Race race;
public:
    Money& money;
    std::vector<CombatUnit*> list_combat_unit;
    std::vector<Colonist*> list_colonist;
    std::vector<Worker*> list_worker;

    UnitFactory(Player which, Map* map, Money& money, Race race);
    ~UnitFactory();

    unsigned int  AddWarrior(City* where);
    unsigned int  AddArcher(City* where);
    unsigned int  AddWizard(City* where);
    unsigned int  AddColonist(City* where);
    unsigned int  AddWorker(City* where);

    std::string Info();
};


template <typename T>
unsigned int PlaceToInsert(std::vector<T> &list_of_unit) {
    unsigned int list_size = list_of_unit.size();
    for(unsigned int i = 0; i < list_size; ++i) {
        if(list_of_unit[i] == nullptr) {
            return i;
        }
    }
    list_of_unit.push_back(nullptr);
    return list_size;
}

#endif //GAME_UNITFACTORY_H

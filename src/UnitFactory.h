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
    Money& money;
    Race race;
public:
    std::vector<CombatUnit*> list_combat_unit;
    std::vector<Colonist*> list_colonist;
    std::vector<Worker*> list_worker;

    UnitFactory(Player which, Map* map, Money& money, Race race);
    ~UnitFactory();

    void AddWarrior(City& where);
    void AddArcher(City& where);
    void AddWizard(City& where);
    void AddColonist(City& where);
    void AddWorker(City& where);

    std::string Info();
};


#endif //GAME_UNITFACTORY_H

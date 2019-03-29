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
    Map& map;
    Money& money;
    Race race;
public:
    std::vector<CombatUnit*> list_combat_unit;
    std::vector<Colonist*> list_colonist;
    std::vector<Worker*> list_worker;

    UnitFactory(Player which, Map& map, Money& money, Race race);
    virtual ~UnitFactory();

    virtual void AddWarrior(City& where) = 0;
    virtual void AddArcher(City& where) = 0;
    virtual void AddWizard(City& where) = 0;
    virtual void AddColonist(City& where);
    virtual void AddWorker(City& where);

    virtual std::string Info();
};


#endif //GAME_UNITFACTORY_H

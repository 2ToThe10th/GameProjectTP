#ifndef GAME_UNITFACTORY_H
#define GAME_UNITFACTORY_H

#include "CombatUnit.h"
#include "City.h"
#include "Enums.h"
#include <iostream>
#include <vector>

class UnitFactory {
private:
    Player which;
public:
    std::vector<Unit> list;

    UnitFactory(Player which);
    ~UnitFactory();

    virtual void AddWarrior(City where) = 0;
    virtual void AddArcher(City where) = 0;
    virtual void AddWizard(City where) = 0;
    virtual void AddColonist(City where) = 0;
    virtual void AddWorker(City where) = 0;
};


#endif //GAME_UNITFACTORY_H
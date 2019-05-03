#include "UnitFactory.h"
#include "Money.h"
#include "Warrior.h"
#include "Archer.h"
#include "Wizard.h"
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

std::string UnitFactory::Info() {
    switch(which) {
        case Me:
            return "Me";
        case Opponent:
            return "Opponent";
    }
    return "";
}

UnitFactory::UnitFactory(enum::Player which, Map& map, Money& money, Race race): which(which), map(map), money(money), race(race) {}

UnitFactory::~UnitFactory() {
    for(CombatUnit* i: list_combat_unit) {
        delete i;
        i = nullptr;
    }
    for(Colonist* i: list_colonist) {
        delete i;
        i = nullptr;
    }
    for(Worker* i: list_worker) {
        delete i;
        i = nullptr;
    }
}


void UnitFactory::AddWarrior(City &where) {
    if(map.combat(where.location) != nullptr) {
        cout << "Other combat unit is already there" << endl;
        cout << map.combat(where.location)->Info() << endl;
        return;
    }

    if(!money.Take(Warrior::cost_wood, Warrior::cost_iron, Warrior::cost_gold)) {
        cout << "You don't have enough money" << endl;
        cout << money.Info() << endl;
        return;
    }

    list_combat_unit.push_back(new Warrior(which, race, 100, where.location, map, 25));

    map.combat(where.location) = list_combat_unit.back();

    //TODO: send message about building unit to opponent
}

void UnitFactory::AddArcher(City &where) {
    if(map.combat(where.location) != nullptr) {
        cout << "Other combat unit is already there" << endl;
        cout << map.combat(where.location)->Info() << endl;
        return;
    }

    if(!where.IsArcherTowerExist()) {
        cout << "Build Archer Town first" << endl;
        return;
    }

    if(!money.Take(Archer::cost_wood, Archer::cost_iron, Archer::cost_gold)) {
        cout << "You don't have enough money" << endl;
        cout << money.Info() << endl;
        return;
    }


    list_combat_unit.push_back(new Archer(which, race, 75, where.location, map, 50));

    map.combat(where.location) = list_combat_unit.back();

    //TODO: send message about building unit to opponent
}

void UnitFactory::AddWizard(City &where) {
    if(map.combat(where.location) != nullptr) {
        cout << "Other combat unit is already there" << endl;
        cout << map.combat(where.location)->Info() << endl;
        return;
    }

    if(!where.IsWizardTowerExist()) {
        cout << "Build Wizard Town first" << endl;
        return;
    }

    if(!money.Take(Wizard::cost_wood, Wizard::cost_iron, Wizard::cost_gold)) {
        cout << "You don't have enough money" << endl;
        cout << money.Info() << endl;
        return;
    }

    list_combat_unit.push_back(new Wizard(which, race, 150, where.location, map, 90));

    map.combat(where.location) = list_combat_unit.back();

    //TODO: send message about building unit to opponent
}


void UnitFactory::AddColonist(City& where) {
    if(map.colonist(where.location) != nullptr || map.worker(where.location) != nullptr) {
        cout << "Other military unit is already there" << endl;
        return;
    }

    if(!money.Take(Colonist::cost_wood, Colonist::cost_iron, Colonist::cost_gold)) {
        cout << "You don't have enough money" << endl;
        cout << money.Info() << endl;
        return;
    }

    list_colonist.push_back(new Colonist(which, race, 50, where.location, map));

    map.colonist(where.location) = list_colonist.back();

    //TODO: send message about building unit to opponent
}

void UnitFactory::AddWorker(City &where) {

    if(map.colonist(where.location) != nullptr || map.worker(where.location) != nullptr) {
        cout << "Other military unit is already there" << endl;
        return;
    }

    if(!money.Take(Worker::cost_wood, Worker::cost_iron, Worker::cost_gold)) {
        cout << "You don't have enough money" << endl;
        cout << money.Info() << endl;
        return;
    }

    list_worker.push_back(new Worker(which, race, 25, where.location, map));

    map.worker(where.location) = list_worker.back();

    //TODO: send message about building unit to opponent
}

#include "UnitFactory.h"
#include "Money.h"
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
    }
    for(Colonist* i: list_colonist) {
        delete i;
    }
    for(Worker* i: list_worker) {
        delete i;
    }
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

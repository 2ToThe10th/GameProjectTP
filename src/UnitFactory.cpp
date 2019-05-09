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

    std::string race_string;

    switch (race) {
        case Water:
            race_string = "Water";
            break;
        case Air:
            race_string = "Air";
            break;
        case Earth:
            race_string = "Earth";
            break;
        case Fire:
            race_string = "Fire";
            break;
    }

    switch (which) {
        case Me:
            return race_string + " Me";
        case Opponent:
            return race_string + " Opponent";
    }

    return "";
}

UnitFactory::UnitFactory(enum::Player which, Map* map, Money& money, Race race): which(which), map(map), money(money), race(race) {}

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

void UnitFactory::AddWarrior(City* where) {
    if(map->combat(where->location) != nullptr) {
        cout << "Other combat unit is already there" << endl;
        cout << map->combat(where->location)->Info() << endl;
        return;
    }

    if(!money.Take(Warrior::cost_wood, Warrior::cost_iron, Warrior::cost_gold)) {
        cout << "You don't have enough money" << endl;
        cout << money.Info() << endl;
        return;
    }

    int health = 0;
    int damage = 0;

    switch (race) {
        case Water:
            health = 100;
            damage = 25;
            break;
        case Earth:
            health = 120;
            damage = 20;
            break;
        case Air:
            health = 80;
            damage = 35;
            break;
        case Fire:
            health = 90;
            damage = 30;
            break;
    }

    unsigned int id = PlaceToInsert<CombatUnit*>(list_combat_unit);

    auto new_warrior = new Warrior(which, race, health, where->location, map, damage, id);

    list_combat_unit[id] = new_warrior;

    map->combat(where->location) = new_warrior;

    //TODO: send message about building unit to opponent
}

void UnitFactory::AddArcher(City* where) {
    if(map->combat(where->location) != nullptr) {
        cout << "Other combat unit is already there" << endl;
        cout << map->combat(where->location)->Info() << endl;
        return;
    }

    if(!where->IsArcherTowerExist()) {
        cout << "Build Archer Town first" << endl;
        return;
    }

    if(!money.Take(Archer::cost_wood, Archer::cost_iron, Archer::cost_gold)) {
        cout << "You don't have enough money" << endl;
        cout << money.Info() << endl;
        return;
    }

    int health = 0;
    int damage = 0;

    switch (race) {
        case Water:
            health = 75;
            damage = 50;
            break;
        case Earth:
            health = 90;
            damage = 40;
            break;
        case Air:
            health = 60;
            damage = 55;
            break;
        case Fire:
            health = 70;
            damage = 50;
            break;
    }


    unsigned int id = PlaceToInsert<CombatUnit*>(list_combat_unit);

    auto new_archer = new Archer(which, race, health, where->location, map, damage, id);

    list_combat_unit[id] = new_archer;

    map->combat(where->location) = new_archer;

    //TODO: send message about building unit to opponent
}

void UnitFactory::AddWizard(City* where) {
    if(map->combat(where->location) != nullptr) {
        cout << "Other combat unit is already there" << endl;
        cout << map->combat(where->location)->Info() << endl;
        return;
    }

    if(!where->IsWizardTowerExist()) {
        cout << "Build Wizard Town first" << endl;
        return;
    }

    if(!money.Take(Wizard::cost_wood, Wizard::cost_iron, Wizard::cost_gold)) {
        cout << "You don't have enough money" << endl;
        cout << money.Info() << endl;
        return;
    }

    int health = 0;
    int damage = 0;

    switch (race) {
        case Water:
            health = 150;
            damage = 50;
            break;
        case Earth:
            health = 170;
            damage = 30;
            break;
        case Air:
            health = 140;
            damage = 70;
            break;
        case Fire:
            health = 160;
            damage = 15;
            break;
    }

    unsigned int id = PlaceToInsert<CombatUnit*>(list_combat_unit);

    auto new_wizard = new Wizard(which, race, health, where->location, map, damage, id);

    list_combat_unit[id] = new_wizard;

    map->combat(where->location) = new_wizard;

    //TODO: send message about building unit to opponent
}


void UnitFactory::AddColonist(City* where) {
    if(map->colonist(where->location) != nullptr || map->worker(where->location) != nullptr) {
        cout << "Other military unit is already there" << endl;
        return;
    }

    if(!money.Take(Colonist::cost_wood, Colonist::cost_iron, Colonist::cost_gold)) {
        cout << "You don't have enough money" << endl;
        cout << money.Info() << endl;
        return;
    }

    unsigned int id = PlaceToInsert<Colonist*>(list_colonist);

    auto new_colonist = new Colonist(which, race, 1, where->location, map, id);

    list_colonist[id] = new_colonist;

    map->colonist(where->location) = new_colonist;

    //TODO: send message about building unit to opponent
}

void UnitFactory::AddWorker(City* where) {

    if(map->colonist(where->location) != nullptr || map->worker(where->location) != nullptr) {
        cout << "Other military unit is already there" << endl;
        return;
    }

    if(!money.Take(Worker::cost_wood, Worker::cost_iron, Worker::cost_gold)) {
        cout << "You don't have enough money" << endl;
        cout << money.Info() << endl;
        return;
    }

    unsigned int id = PlaceToInsert<Worker*>(list_worker);
    
    auto new_worker = new Worker(which, race, 1, where->location, map, id);

    list_worker[id] = new_worker;

    map->worker(where->location) = new_worker;

    //TODO: send message about building unit to opponent
}

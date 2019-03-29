#include "AirUnitFactory.h"
#include "Warrior.h"
#include "Archer.h"
#include "Wizard.h"

using std::cout;
using std::endl;

std::string AirUnitFactory::Info() {
    return (UnitFactory::Info() + " Air");
}

AirUnitFactory::AirUnitFactory(Player which, Map& map, Money& money) : UnitFactory(which, map, money, Race::Air) {}

void AirUnitFactory::AddWarrior(City &where) {
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

void AirUnitFactory::AddArcher(City &where) {
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

void AirUnitFactory::AddWizard(City &where) {
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

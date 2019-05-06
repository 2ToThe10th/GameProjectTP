#include <iostream>
#include "CombatUnit.h"
#include "Map.h"

using std::to_string;

CombatUnit::CombatUnit(Player which, Race race, int health, Location city_location, Map* map, int damage):
            Unit::Unit(which, race, health, city_location, map), damage(damage) {}


std::string CombatUnit::Info() {
    return Unit::Info() + " damage: " + to_string(damage);
}

void CombatUnit::Go(Direction where) {
    Location new_location = location.Direction(where);

    if(!new_location.IsOnField()) {
        std::cout << "Units can go only in the fields" << std::endl;
        return;
    }
    if(map->combat(new_location) != nullptr) {
        std::cout << "only one combat unit can be on the field" << std::endl;
        return;
    }

    map->combat(new_location) = map->combat(location);
    map->combat(location) = nullptr;
    location = new_location;
}

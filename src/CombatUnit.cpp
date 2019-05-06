#include "CombatUnit.h"

using std::to_string;

CombatUnit::CombatUnit(Player which, Race race, int health, Location city_location, Map* map, int damage):
            Unit::Unit(which, race, health, city_location, map), damage(damage) {}


std::string CombatUnit::Info() {
    return Unit::Info() + " damage: " + to_string(damage);
}

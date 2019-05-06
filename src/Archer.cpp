#include "Archer.h"

Archer::Archer(Player which, Race race, int health, Location city_location, Map* map, int damage, unsigned int id) :
        CombatUnit(which, race, health, city_location, map, damage, id) {}

std::string Archer::Info() {
    return "Archer " + CombatUnit::Info();
}

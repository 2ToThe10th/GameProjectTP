#include "Warrior.h"

Warrior::Warrior(Player which, Race race, int health, Location city_location, Map*& map, int damage) :
        CombatUnit(which, race, health, city_location, map, damage) {}

std::string Warrior::Info() {
    return "Warrior " + CombatUnit::Info();
}

#include "Wizard.h"

Wizard::Wizard(Player which, Race race, int health, Location city_location, Map*& map, int damage) :
        CombatUnit(which, race, health, city_location, map, damage) {}

std::string Wizard::Info() {
    return "Wizard " + CombatUnit::Info();
}

#include "Unit.h"
#include <string>

using std::to_string;

UnitFactory* Unit::my_unit_factory = nullptr;
UnitFactory* Unit::opponent_unit_factory = nullptr;

std::string Unit::Info() {
    return "health: " + to_string(health) + " of MAX_HEALTH: "  + to_string(MAX_HEALTH) + " location: " + location.Info() +
           " already_move: " + to_string(already_move) + " frozen: " + to_string(frozen);
}

Unit::Unit(Player which, Race race, int health, Location city_location, Map *map, unsigned int id) :
        which(which), race(race), MAX_HEALTH(health), health(health), location(city_location), map(map), id(id) {}

Location Unit::GetLocation() const {
    return location;
}


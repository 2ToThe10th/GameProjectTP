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
           location(city_location), map(map), MAX_HEALTH(health), health(health), which(which), id(id), race(race) {}

Location Unit::GetLocation() const {
    return location;
}

void Unit::NewTurn() {
    if(frozen) {
        frozen = false;
        already_move = true;
    }
    else {
        already_move = false;
    }
}

void Unit::Froze() {
    frozen = true;
}


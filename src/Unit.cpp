#include "Unit.h"
#include <string>

using std::to_string;

std::string Unit::Info() {
    return "health: " + to_string(health) + " of MAX_HEALTH: "  + to_string(MAX_HEALTH) + " location: " + location.Info() +
           " already_move: " + to_string(already_move) + " frozen: " + to_string(frozen);
}

Unit::Unit(Player which, Race race, int health, Location city_location, Map* map):
           MAX_HEALTH(health), health(health), location(city_location), map(map), race(race), which(which) {}

Location Unit::GetLocation() const {
    return location;
}


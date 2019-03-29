#include "Colonist.h"

Colonist::Colonist(Player which, Race race, int health, Location city_location, Map &map) :
          Unit(which, race, health, city_location, map) {}

std::string Colonist::Info() {
    return "Colonist " + Unit::Info();
}

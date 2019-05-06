#include <iostream>
#include "Colonist.h"
#include "Map.h"

Colonist::Colonist(Player which, Race race, int health, Location city_location, Map* map) :
          Unit(which, race, health, city_location, map) {}

std::string Colonist::Info() {
    return "Colonist " + Unit::Info();
}

void Colonist::Go(Direction where) {
    Location new_location = location.Direction(where);

    if(!new_location.IsOnField()) {
        std::cout << "Units can go only in the fields" << std::endl;
        return;
    }
    if(map->colonist(new_location) != nullptr || map->worker(new_location) != nullptr) {
        std::cout << "only one military unit can be on the field" << std::endl;
        return;
    }

    map->colonist(new_location) = map->colonist(location);
    map->colonist(location) = nullptr;
    location = new_location;
}

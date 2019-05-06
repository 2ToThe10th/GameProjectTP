#include <iostream>
#include "Worker.h"
#include "Map.h"

Worker::Worker(Player which, Race race, int health, Location city_location, Map* map) :
        Unit(which, race, health, city_location, map) {}

std::string Worker::Info() {
    return "Worker " + Unit::Info();
}

void Worker::Go(Direction where) {
    Location new_location = location.Direction(where);

    if(!new_location.IsOnField()) {
        std::cout << "Units can go only in the fields" << std::endl;
        return;
    }
    if(map->colonist(new_location) != nullptr || map->worker(new_location) != nullptr) {
        std::cout << "only one military unit can be on the field" << std::endl;
        return;
    }

    map->worker(new_location) = map->worker(location);
    map->worker(location) = nullptr;
    location = new_location;
}

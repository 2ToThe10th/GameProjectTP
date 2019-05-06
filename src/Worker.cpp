#include <iostream>
#include "Worker.h"
#include "Map.h"
#include "CombatUnit.h"

Worker::Worker(Player which, Race race, int health, Location city_location, Map* map, unsigned int id) :
        Unit(which, race, health, city_location, map, id) {}

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
    if(map->combat(new_location) != nullptr && map->combat(new_location)->which != which) {
        std::cout << "You could not go to field with enemy unit by worker" << std::endl;
        return;
    }

    map->worker(new_location) = map->worker(location);
    map->worker(location) = nullptr;
    location = new_location;
}

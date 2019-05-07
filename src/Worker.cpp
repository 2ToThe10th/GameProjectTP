#include <iostream>
#include "Worker.h"
#include "Map.h"
#include "CombatUnit.h"
#include "UnitFactory.h"

Worker::Worker(Player which, Race race, int health, Location city_location, Map* map, unsigned int id) :
        Unit(which, race, health, city_location, map, id) {}

std::string Worker::Info() {
    return "Worker " + Unit::Info();
}

void Worker::Go(Direction where) {
    Location new_location = location.Direction(where);

    if(already_move) {
        std::cout << "This worker has already moved this turn" << std::endl;
        return;
    }
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
    already_move = true;
}

Worker::~Worker() {
    if(which == Player::Me) {
        my_unit_factory->list_worker[id] = nullptr;
    }
    else {
        opponent_unit_factory->list_worker[id] = nullptr;
    }
    map->worker(location) = nullptr;
}

#include <iostream>
#include "Worker.h"
#include "Map.h"
#include "CombatUnit.h"
#include "UnitFactory.h"

Money* Worker::money = nullptr;

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

    if(map->city(location) != nullptr && map->city(location)->which == which) {
        money->Add(cary_gold, cary_silver, cary_wood);
        cary_gold = 0;
        cary_silver = 0;
        cary_wood = 0;
    }

    already_move = true;
}

void Worker::Mine() {
    if(cary_gold > 0 || cary_silver > 0 || cary_wood > 0) {
        std::cout << "Can not mine resource because this worker already care some" << std::endl;
        return;
    }

    if(map->resource(location) == Resource::Nothing) {
        std::cout << "No resources here" << std::endl;
        return;
    }

    if(map->resource(location) == Resource::Gold) {
        cary_gold = 100;
    }
    else if(map->resource(location) == Resource::Silver) {
        cary_silver = 100;
    }
    else if(map->resource(location) == Resource::Wood) {
        cary_wood = 100;
    }
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

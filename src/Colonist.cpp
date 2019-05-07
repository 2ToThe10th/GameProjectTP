#include <iostream>
#include "Colonist.h"
#include "Map.h"
#include "CombatUnit.h"
#include "UnitFactory.h"

Colonist::Colonist(Player which, Race race, int health, Location city_location, Map* map, unsigned int id) :
        Unit(which, race, health, city_location, map, id) {}

std::string Colonist::Info() {
    return "Colonist " + Unit::Info();
}

void Colonist::Go(Direction where) {
    Location new_location = location.Direction(where);

    if(already_move) {
        std::cout << "This colonist has already moved this turn" << std::endl;
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
        std::cout << "You could not go to field with enemy unit by colonist" << std::endl;
        return;
    }


    map->colonist(new_location) = map->colonist(location);
    map->colonist(location) = nullptr;
    location = new_location;
    already_move = true;
}

Colonist::~Colonist() {
    if(which == Player::Me) {
        my_unit_factory->list_colonist[id] = nullptr;
    }
    else {
        opponent_unit_factory->list_colonist[id] = nullptr;
    }
    map->colonist(location) = nullptr;
}

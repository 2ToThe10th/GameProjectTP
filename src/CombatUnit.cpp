#include <iostream>
#include "CombatUnit.h"
#include "Colonist.h"
#include "Map.h"
#include "UnitFactory.h"

using std::to_string;

CombatUnit::CombatUnit(Player which, Race race, int health, Location city_location, Map* map, int damage, unsigned int id):
        Unit::Unit(which, race, health, city_location, map, id), damage(damage) {}


std::string CombatUnit::Info() {
    return Unit::Info() + " damage: " + to_string(damage);
}

void CombatUnit::Go(Direction where) {
    Location new_location = location.Direction(where);

    if(!new_location.IsOnField()) {
        std::cout << "Units can go only in the fields" << std::endl;
        return;
    }
    if(map->combat(new_location) != nullptr) {
        std::cout << "only one combat unit can be on the field" << std::endl;
        return;
    }
    if(map->colonist(new_location) != nullptr && map->colonist(new_location)->which != which) {
        if(which == Player::Me) {
            unsigned int id = PlaceToInsert(my_unit_factory->list_colonist);
            my_unit_factory->list_colonist[id] = opponent_unit_factory->list_colonist[map->colonist(new_location)->id];
            opponent_unit_factory->list_colonist[map->colonist(new_location)->id] = nullptr;
            map->colonist(new_location)->id = id;
        }
        else {
            unsigned int id = PlaceToInsert(opponent_unit_factory->list_colonist);
            opponent_unit_factory->list_colonist[id] = my_unit_factory->list_colonist[map->colonist(new_location)->id];
            my_unit_factory->list_colonist[map->colonist(new_location)->id] = nullptr;
            map->colonist(new_location)->id = id;
        }
        map->colonist(new_location)->which = which;
    }
    if(map->worker(new_location) != nullptr && map->worker(new_location)->which != which) {
        if(which == Player::Me) {
            unsigned int id = PlaceToInsert(my_unit_factory->list_worker);
            my_unit_factory->list_worker[id] = opponent_unit_factory->list_worker[map->worker(new_location)->id];
            opponent_unit_factory->list_worker[map->worker(new_location)->id] = nullptr;
            map->worker(new_location)->id = id;
        }
        else {
            unsigned int id = PlaceToInsert(opponent_unit_factory->list_worker);
            opponent_unit_factory->list_worker[id] = my_unit_factory->list_worker[map->worker(new_location)->id];
            my_unit_factory->list_worker[map->worker(new_location)->id] = nullptr;
            map->worker(new_location)->id = id;
        }
        map->worker(new_location)->which = which;
    }

    map->combat(new_location) = map->combat(location);
    map->combat(location) = nullptr;
    location = new_location;
}

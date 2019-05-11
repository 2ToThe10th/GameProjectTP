#include <iostream>
#include "CombatUnit.h"
#include "Colonist.h"
#include "Map.h"
#include "UnitFactory.h"
#include "Unit.h"
#include "City.h"
#include "CityFactory.h"

using std::to_string;

CombatUnit::CombatUnit(Player which, Race race, int health, Location city_location, Map* map, int damage, unsigned int id):
        Unit::Unit(which, race, health, city_location, map, id), damage(damage) {}


std::string CombatUnit::Info() {
    return Unit::Info() + " damage: " + to_string(damage);
}

unsigned int CombatUnit::Go(Direction where) {
    Location new_location = location.Direction(where);

    if(already_move) {
        std::cout << "This combat has already moved this turn" << std::endl;
        return 1;
    }
    if(!new_location.IsOnField()) {
        std::cout << "Units can go only in the fields" << std::endl;
        return 1;
    }
    if(map->combat(new_location) != nullptr) {
        std::cout << "only one combat unit can be on the field" << std::endl;
        return 1;
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

    if(map->city(location) != nullptr && map->city(location)->which != which) {
        auto attacked_city = map->city(location);

        CityFactory* my_city_factory = nullptr;
        CityFactory* opponent_city_factory = nullptr;

        if(which == Player::Me) {
            my_city_factory = City::my_city_factory;
            opponent_city_factory = City::opponent_city_factory;
        }
        else {
            opponent_city_factory = City::my_city_factory;
            my_city_factory = City::opponent_city_factory;
        }
        opponent_city_factory->cities[attacked_city->id] = nullptr;
        unsigned int new_id = PlaceToInsert(my_city_factory->cities);
        my_city_factory->cities[new_id] = attacked_city;
        attacked_city->id = new_id;
        attacked_city->which = which;
    }

    already_move = true;

    return 0;
}

CombatUnit::~CombatUnit() {
    if(which == Player::Me) {
        my_unit_factory->list_combat_unit[id] = nullptr;
    }
    else {
        opponent_unit_factory->list_combat_unit[id] = nullptr;
    }
    map->combat(location) = nullptr;
    if(health <= 0) {
        if(which == Player::Me) {
            std::cout << "Combat id=" + std::to_string(id) + " die." << std::endl;
        }
        else {
            std::cout << "Enemy combat id=" + std::to_string(id) + " die." << std::endl;
        }
    }
}

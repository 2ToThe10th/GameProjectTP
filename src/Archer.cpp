#include <iostream>
#include "Archer.h"
#include "Map.h"
#include "Colonist.h"
#include "Worker.h"

Archer::Archer(Player which, Race race, int health, Location city_location, Map* map, int damage, unsigned int id) :
        CombatUnit(which, race, health, city_location, map, damage, id, CombatUnitType::ArcherCombatType) {}

std::string Archer::Info() {
    return "Archer " + CombatUnit::Info();
}

unsigned int Archer::Attack(std::vector<Direction> &where_attack) {

    if(already_move) {
        std::cout << "This combat unit has already moved this turn" << std::endl;
        return 1;
    }
    if(where_attack.empty() || where_attack.size() > 2) {
        std::cout << "Archer can attack only fields on distance one or two" << std::endl;
        return 1;
    }

    Location location_to_attack = location;
    for(auto i: where_attack) {
        location_to_attack = location_to_attack.Direction(i);
    }

    if(!location_to_attack.IsOnField()) {
        std::cout << "You can attack only on field" << std::endl;
        return 1;
    }
    if(map->combat(location_to_attack) == nullptr || map->combat(location_to_attack)->which == which) {
        if(map->colonist(location_to_attack) != nullptr && map->colonist(location_to_attack)->which != which) {
            map->colonist(location_to_attack)->health = 0;
            delete map->colonist(location_to_attack);
            already_move = true;
        }
        else if(map->worker(location_to_attack) != nullptr && map->worker(location_to_attack)->which != which) {
            map->worker(location_to_attack)->health = 0;
            delete map->worker(location_to_attack);
            already_move = true;
        } else {
            std::cout << "No enemy unit on field to attack" << std::endl;
            return 1;
        }
        return 0;
    }

    auto enemy_unit = map->combat(location_to_attack);
    enemy_unit->health -= damage;

    if(enemy_unit->health <= 0) {
        delete enemy_unit;
    }

    already_move = true;
    return 0;
}

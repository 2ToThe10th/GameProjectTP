#include <iostream>
#include "Warrior.h"
#include "Map.h"

Warrior::Warrior(Player which, Race race, int health, Location city_location, Map* map, int damage, unsigned int id) :
        CombatUnit(which, race, health, city_location, map, damage, id) {}

std::string Warrior::Info() {
    return "Warrior " + CombatUnit::Info();
}

void Warrior::Attack(std::vector<Direction>& where_attack) {

    if(already_move) {
        std::cout << "This combat unit has already moved this turn" << std::endl;
        return;
    }
    if(where_attack.size() != 1) {
        std::cout << "Warrior can attack only fields exactly near to him" << std::endl;
        return;
    }

    Location location_to_attack = location.Direction(where_attack[0]);

    if(!location_to_attack.IsOnField()) {
        std::cout << "You can attack only on field" << std::endl;
        return;
    }
    if(map->combat(location_to_attack) == nullptr || map->combat(location_to_attack)->which == which) {
        std::cout << "No enemy combat unit on field to attack" << std::endl;
        return;
    }

    auto enemy = map->combat(location_to_attack);



    health -= enemy->damage;
    enemy->health -= damage;

    bool is_die = false;

    if(enemy->health <= 0) {
        delete enemy;
        is_die = true;
    }

    if(health <= 0) {
        delete this;
    }
    else {
        if(is_die) {
            Go(where_attack[0]);
        }
        already_move = true;
    }

}

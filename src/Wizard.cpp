#include <iostream>
#include "Wizard.h"
#include "Map.h"
#include "Worker.h"
#include "Colonist.h"
#include "CombatUnit.h"

Wizard::Wizard(Player which, Race race, int health, Location city_location, Map* map, int damage, unsigned int id) :
        CombatUnit(which, race, health, city_location, map, damage, id) {}

std::string Wizard::Info() {
    return "Wizard " + CombatUnit::Info();
}

unsigned int Wizard::Attack(std::vector<Direction> &where_attack) {

    if(already_move) {
        std::cout << "This combat unit has already moved this turn" << std::endl;
        return 1;
    }
    if(where_attack.empty() || where_attack.size() > 3) {
        std::cout << "Wizard can use their abilities only on fields on distance between one and three" << std::endl;
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

    if(race == Race::Water) {

        if (map->combat(location_to_attack) == nullptr || map->combat(location_to_attack)->which == which) {
            if (map->colonist(location_to_attack) != nullptr && map->colonist(location_to_attack)->which != which) {
                map->colonist(location_to_attack)->health = 0;
                delete map->colonist(location_to_attack);
                already_move = true;
            } else if (map->worker(location_to_attack) != nullptr &&
                       map->worker(location_to_attack)->which != which) {
                map->worker(location_to_attack)->health = 0;
                delete map->worker(location_to_attack);
                already_move = true;
            } else {
                std::cout << "No enemy unit on field to attack and froze" << std::endl;
                return 1;
            }
            return 0;
        }

        auto enemy_unit = map->combat(location_to_attack);
        enemy_unit->health -= damage;

        if (enemy_unit->health <= 0) {
            delete enemy_unit;
        } else {
            enemy_unit->Froze();
        }
    }
    else if(race == Race::Earth) {
        
        if (map->combat(location_to_attack) == nullptr || map->combat(location_to_attack)->which == which) {
            if (map->colonist(location_to_attack) != nullptr && map->colonist(location_to_attack)->which != which) {
                map->colonist(location_to_attack)->health = 0;
                delete map->colonist(location_to_attack);
            } else if (map->worker(location_to_attack) != nullptr &&
                       map->worker(location_to_attack)->which != which) {
                map->worker(location_to_attack)->health = 0;
                delete map->worker(location_to_attack);
            } 
        }
        else {
            auto enemy_unit = map->combat(location_to_attack);
            enemy_unit->health -= damage;

            if (enemy_unit->health <= 0) {
                delete enemy_unit;
            }
        }
        
        Direction sides[] = {Direction::Down, Direction::Left, Direction::Right, Direction::Up};
        
        for(auto side: sides) {
            Location location_for_side_attack = location_to_attack.Direction(side);

            if (map->combat(location_for_side_attack) == nullptr || map->combat(location_for_side_attack)->which == which) {
                if (map->colonist(location_for_side_attack) != nullptr && map->colonist(location_for_side_attack)->which != which) {
                    map->colonist(location_for_side_attack)->health = 0;
                    delete map->colonist(location_for_side_attack);
                } else if (map->worker(location_for_side_attack) != nullptr &&
                           map->worker(location_for_side_attack)->which != which) {
                    map->worker(location_for_side_attack)->health = 0;
                    delete map->worker(location_for_side_attack);
                }
            }
            else {
                auto enemy_unit = map->combat(location_for_side_attack);
                enemy_unit->health -= damage / 2;

                if (enemy_unit->health <= 0) {
                    delete enemy_unit;
                }
            }
        }
    }
    else if(race == Race::Air) {
        if (map->combat(location_to_attack) == nullptr || map->combat(location_to_attack)->which != which) {
            std::cout << "No your combat unit on field to heal" << std::endl;
            return 1;
        }
        auto friend_unit = map->combat(location_to_attack);
        std::cout << friend_unit->health << std::endl;
        friend_unit->health = std::min(friend_unit->health + damage, friend_unit->MAX_HEALTH);
    }
    else if(race == Race::Fire) {

        Direction direction_to_attack[] = {Direction::Up, Direction::Right, Direction::Down, Direction::Down, Direction::Left, Direction::Left, Direction::Up, Direction::Up};

        for(auto next_direction: direction_to_attack) {

            location_to_attack = location_to_attack.Direction(next_direction);

            if (map->combat(location_to_attack) == nullptr || map->combat(location_to_attack)->which == which) {
                if (map->colonist(location_to_attack) != nullptr && map->colonist(location_to_attack)->which != which) {
                    map->colonist(location_to_attack)->health = 0;
                    delete map->colonist(location_to_attack);
                } else if (map->worker(location_to_attack) != nullptr &&
                           map->worker(location_to_attack)->which != which) {
                    map->worker(location_to_attack)->health = 0;
                    delete map->worker(location_to_attack);
                }
            } else {
                auto enemy_unit = map->combat(location_to_attack);
                enemy_unit->health -= damage;

                if (enemy_unit->health <= 0) {
                    delete enemy_unit;
                }
            }
        }
    }

    already_move = true;
    return 0;
}

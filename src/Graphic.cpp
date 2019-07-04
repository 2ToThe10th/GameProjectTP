#include "Graphic.h"
#include "CombatUnit.h"
#include "Worker.h"
#include "Colonist.h"

Graphic::Graphic(Map *map): map(map) {}

//|--------
//|ComUCity
//|MilUresr


void Graphic::Draw() {
    for(int i = Map::MAX_HEIGHT - 1; i >= 0; --i) {
        for(int j = 0; j < Map::MAX_WIDTH; ++j) {
            cout << "|--------";
        }
        cout << "|\n";
        for(int j = 0; j < Map::MAX_WIDTH; ++j) {
            cout << "|";
            if(map->combat(Location(i, j)) == nullptr) {
                cout << empty_place;
            }
            else {
                if(map->combat(Location(i, j))->which == Player::Me) {
                    cout << set_green_background_color;
                }
                else {
                    cout << set_red_background_color;
                }

                unsigned int id = map->combat(Location(i, j))->id;
                if(id < 100) {
                    cout << "0";
                }
                if(id < 10) {
                    cout << "0";
                }
                cout << id;
                switch (map->combat(Location(i, j))->combat_unit_type) {
                    case CombatUnitType::WarriorCombatType:
                        cout << "S";
                        break;
                    case CombatUnitType::ArcherCombatType:
                        cout << "A";
                        break;
                    case CombatUnitType::WizardCombatType:
                        cout << "W";
                        break;
                }

                cout << set_default_background_color;
            }
            if(map->city(Location(i, j)) == nullptr) {
                cout << empty_place;
            }
            else {
                if(map->city(Location(i, j))->which == Player::Me) {
                    cout << set_green_background_color;
                }
                else {
                    cout << set_red_background_color;
                }

                unsigned int id = map->city(Location(i, j))->id;
                if(id < 100) {
                    cout << "0";
                }
                if(id < 10) {
                    cout << "0";
                }
                cout << id;

                if(map->city(Location(i, j))->IsWizardTowerExist()) {
                    if (map->city(Location(i, j))->which == Player::Me) {
                        cout << set_blue_background_color;
                    } else {
                        cout << set_cyan_background_color;
                    }
                }
                else if(map->city(Location(i, j))->IsArcherTowerExist()) {
                    if(map->city(Location(i, j))->which == Player::Me) {
                        cout << set_yellow_background_color;
                    }
                    else {
                        cout << set_magenta_background_color;
                    }
                }

                cout << "C";

                cout << set_default_background_color;
            }
        }
        cout << "|\n";
        for(int j = 0; j < Map::MAX_WIDTH; ++j) {
            cout << "|";
            if(map->colonist(Location(i, j)) == nullptr && map->worker((Location(i,j))) == nullptr) {
                cout << empty_place;
            }
            else {
                if(map->colonist(Location(i, j)) == nullptr) {
                    if(map->worker(Location(i, j))->which == Player::Me) {
                        cout << set_green_background_color;
                    }
                    else {
                        cout << set_red_background_color;
                    }

                    unsigned int id = map->worker(Location(i, j))->id;
                    if (id < 100) {
                        cout << "0";
                    }
                    if (id < 10) {
                        cout << "0";
                    }
                    cout << id;
                    cout << "W";
                }
                else {
                    if(map->colonist(Location(i, j))->which == Player::Me) {
                        cout << set_green_background_color;
                    }
                    else {
                        cout << set_red_background_color;
                    }

                    unsigned int id = map->colonist(Location(i, j))->id;
                    if (id < 100) {
                        cout << "0";
                    }
                    if (id < 10) {
                        cout << "0";
                    }
                    cout << id;
                    cout << "C";
                }

                cout << set_default_background_color;
            }
            if(map->resource(Location(i, j)) == Resource::Nothing) {
                cout << empty_place;
            }
            else {
                cout << "   ";
                switch (map->resource(Location(i, j))) {
                    case Resource::Gold:
                        cout << "G";
                        break;
                    case Resource::Silver:
                        cout << "S";
                        break;
                    case Resource::Wood:
                        cout << "W";
                        break;
                }
            }
        }
        cout << "|\n";
    }
    for(int j = 0; j < Map::MAX_WIDTH; ++j) {
        cout << "|--------";
    }
    cout << "|";
    cout << std::endl;
}

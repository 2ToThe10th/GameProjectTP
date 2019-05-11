#include <string>
#include <cassert>
#include <random>
#include <ctime>
#include "Map.h"
#include "CombatUnit.h"
#include "Colonist.h"
#include "Worker.h"
#include "City.h"

using std::string;
using std::to_string;

Map::Map() {
    for(int i = 0; i < Map::MAX_HEIGHT; ++i) {
        for(int j = 0; j < Map::MAX_WIDTH; ++j) {
            map_combat[i][j] = nullptr;
            map_colonist[i][j] = nullptr;
            map_worker[i][j] = nullptr;
            map_city[i][j] = nullptr;
        }
    }
}

string Map::Info() {
    string ret;
    ret += "Combat\n";

    for(int i = 0; i < Map::MAX_HEIGHT; ++i) {
        for(int j =0; j < Map::MAX_WIDTH; ++j) {
            ret += to_string(i) + " " + to_string(j) + " ";
            if(map_combat[i][j] == nullptr) {
                ret += "nullptr\n";
            } else {
                ret += map_combat[i][j]->Info() + "\n";
            }
        }
    }

    ret += "Colonist\n";

    for(int i = 0; i < Map::MAX_HEIGHT; ++i) {
        for(int j =0; j < Map::MAX_WIDTH; ++j) {
            ret += to_string(i) + " " + to_string(j) + " ";
            if(map_colonist[i][j] == nullptr) {
                ret += "nullptr\n";
            } else {
                ret += map_colonist[i][j]->Info() + "\n";
            }
        }
    }

    ret += "Worker\n";

    for(int i = 0; i < Map::MAX_HEIGHT; ++i) {
        for(int j =0; j < Map::MAX_WIDTH; ++j) {
            ret += to_string(i) + " " + to_string(j) + " ";
            if(map_worker[i][j] == nullptr) {
                ret += "nullptr\n";
            } else {
                ret += map_worker[i][j]->Info() + "\n";
            }
        }
    }

    ret += "Cities\n";

    for(int i = 0; i < Map::MAX_HEIGHT; ++i) {
        for(int j =0; j < Map::MAX_WIDTH; ++j) {
            ret += to_string(i) + " " + to_string(j) + " ";
            if(map_city[i][j] == nullptr) {
                ret += "nullptr\n";
            } else {
                ret += "City\n";
            }
        }
    }

    return ret;
}

CombatUnit*& Map::combat(Location location) {
    assert(location.IsOnField());
    return map_combat[location.x][location.y];
}

Colonist*& Map::colonist(Location location) {
    assert(location.IsOnField());
    return map_colonist[location.x][location.y];
}

Worker*& Map::worker(Location location) {
    assert(location.IsOnField());
    return map_worker[location.x][location.y];
}

City*& Map::city(Location location) {
    assert(location.IsOnField());
    return map_city[location.x][location.y];
}

Resource Map::resource(Location location) {
    assert(location.IsOnField());
    return map_resource[location.x][location.y];
}

void Map::Generate(unsigned int seed) {
    // ____ |______
    // |    |
    // |

    std::mt19937 generate_random(seed);

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j = 0; j < MAX_WIDTH / 2; ++j) {
            int new_random = generate_random() % 100;

            if(new_random < GOLD) {
                map_resource[i][j] = Resource::Gold;
            }
            else if(new_random < GOLD + SILVER) {
                map_resource[i][j] = Resource::Silver;
            }
            else if(new_random < GOLD + SILVER + WOOD) {
                map_resource[i][j] = Resource::Wood;
            }
            else {
                map_resource[i][j] = Resource::Nothing;
            }

            map_resource[i][MAX_HEIGHT - j - 1] = map_resource[i][j];
        }
    }

}


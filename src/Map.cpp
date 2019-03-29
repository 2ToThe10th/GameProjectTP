#include "Map.h"
#include <string>

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
    return map_combat[location.x][location.y];
}

Colonist*& Map::colonist(Location location) {
    return map_colonist[location.x][location.y];
}

Worker*& Map::worker(Location location) {
    return map_worker[location.x][location.y];
}

City*& Map::city(Location location) {
    return map_city[location.x][location.y];
}

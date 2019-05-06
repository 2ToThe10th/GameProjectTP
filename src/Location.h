#ifndef GAME_LOCATION_H
#define GAME_LOCATION_H


#include <string>
#include "Enums.h"

class Map;

class Location {
public:
    int x;
    int y;
    explicit Location(int x, int y);
    Location(const Location& other_location);
    Location& operator=(const Location& other_location);
    const bool operator== (const Location& other_location) const;

    Location Direction(Direction where);
    bool IsOnField();

    std::string Info();
};


#endif //GAME_LOCATION_H

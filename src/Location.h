#ifndef GAME_LOCATION_H
#define GAME_LOCATION_H


#include "Enums.h"

class Location {
private:
    int x;
    int y;
public:
    explicit Location(int x, int y);
    Location(const Location& other_location);
    Location& operator=(const Location& other_location);

    Location Direction(Direction where);
};


#endif //GAME_LOCATION_H

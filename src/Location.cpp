#include "Location.h"
#include "Enums.h"
#include "Map.h"

Location::Location(int x, int y): x(x), y(y) {}

Location::Location(const Location &other_location): x(other_location.x), y(other_location.y) {}

Location &Location::operator=(const Location &other_location) {

    x = other_location.x;
    y = other_location.y;

    return (*this);
}

Location Location::Direction(enum::Direction where) {

    int x_new = x;
    int y_new = y;

    switch(where) {
        case Direction::Up:
            ++x_new;
            break;
        case Direction::Right:
            ++y_new;
            break;
        case Direction::Left:
            --y_new;
            break;
        case Direction::Down:
            --x_new;
            break;
    }

    return Location(x_new, y_new);
}

const bool Location::operator==(const Location &other_location) const {
    return ((x == other_location.x) && (y == other_location.y));
}

std::string Location::Info() {
    return " x:" + std::to_string(x) + " y:" + std::to_string(y);
}

bool Location::IsOnField() {
    return !(x < 0 || x >= Map::MAX_HEIGHT || y < 0 || y >= Map::MAX_WIDTH);
}

#include "Worker.h"

Worker::Worker(Player which, Race race, int health, Location city_location, Map*& map) :
        Unit(which, race, health, city_location, map) {}

std::string Worker::Info() {
    return "Worker " + Unit::Info();
}

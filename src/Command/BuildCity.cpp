#include "BuildCity.h"

unsigned int BuildCity::Do() {
    CityFactory* city_factory = nullptr;
    UnitFactory* unit_factory = nullptr;

    if(which_turn == Player::Me) {
        city_factory = ICommand::my_city_factory;
        unit_factory = ICommand::my_unit_factory;
    }
    else {
        city_factory = ICommand::opponent_ciy_factory;
        unit_factory = ICommand::opponent_unit_factory;

    }
    return city_factory->AddCity(unit_factory->list_colonist[colonist_id]);
}

BuildCity::BuildCity(unsigned int colonist_id): colonist_id(colonist_id) {}


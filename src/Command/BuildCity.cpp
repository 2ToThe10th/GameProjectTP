#include "BuildCity.h"

unsigned int BuildCity::Do() {
    CityFactory* city_factory = nullptr;
    UnitFactory* unit_factory = nullptr;

    if(which_turn == Player::Me) {
        city_factory = ICommand::my_city_factory;
        unit_factory = ICommand::my_unit_factory;
    }
    else {
        city_factory = ICommand::opponent_city_factory;
        unit_factory = ICommand::opponent_unit_factory;

    }

    if(colonist_id < 0 || colonist_id >= unit_factory->list_colonist.size()) {
        std::cout << "No colonist with this id" << std::endl;
        return 1;
    }

    return city_factory->AddCity(unit_factory->list_colonist[colonist_id]);
}

BuildCity::BuildCity(unsigned int colonist_id): colonist_id(colonist_id) {}

void BuildCity::Send() {
    string string_to_send = "buildcity " + std::to_string(colonist_id);
    socket->Write(string_to_send);
}


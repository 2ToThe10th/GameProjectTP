#include <string>
#include <iostream>
#include "CityFactory.h"
#include "Map.h"
#include "UnitFactory.h"
#include "Colonist.h"

using std::string;


CityFactory::CityFactory(Player which, Map* map): which_city_factory(which), map(map) {}


string CityFactory::Info() {
    string str_which;
    switch (which_city_factory) {
        case Player::Me:
            str_which = "Me";
            break;
        case Player::Opponent:
            str_which = "Opponent";
            break;
    }

    return str_which + " City";
}

unsigned int CityFactory::AddCity(Colonist* colonist) {

    if(colonist == nullptr) {
        std::cout << "This colonist is already die. Please, choose another one" << std::endl;
        return 1;
    }

    if(map->city(colonist->GetLocation()) != nullptr) {
        std::cout << "Colonist now in the city. Please, move him." << std::endl;
        return 1;
    }

    unsigned int new_city_id = PlaceToInsert(cities);
    cities[new_city_id] = new City(colonist->GetLocation());
    map->city(colonist->GetLocation()) = cities[new_city_id];
    cities[new_city_id]->id = new_city_id;
    cities[new_city_id]->which = which_city_factory;

    delete colonist;
    return 0;
}

CityFactory::~CityFactory() {
    for(auto i: cities) {
        delete i;
    }
}

#include <string>
#include <iostream>
#include "CityFactory.h"
#include "Map.h"

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

void CityFactory::AddCity(const Colonist* colonist) {

    if(colonist == nullptr) {
        std::cout << "This colonist is already die. Please, choose another one" << std::endl;
        return;
    }

    if(map->city(colonist->GetLocation()) != nullptr) {
        std::cout << "Colonist now in the city. Please, move him." << std::endl;
        return;
    }

    cities.push_back(new City(colonist->GetLocation()));
    map->city(colonist->GetLocation()) = cities.back();

    delete colonist;
}

CityFactory::~CityFactory() {
    for(auto i: cities) {
        delete i;
    }
}

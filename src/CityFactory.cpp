#include "CityFactory.h"
#include <string>

using std::string;


CityFactory::CityFactory(Player which): which_city_factory(which) {}


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

void CityFactory::AddCity(Colonist colonist) {

}

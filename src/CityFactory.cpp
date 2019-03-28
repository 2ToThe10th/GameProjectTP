#include "CityFactory.h"
#include <string>

using std::string;


CityFactory::CityFactory(Player which): which_city_factory(which) {}


string CityFactory::Info() {
    string str_gold_amt;
    int gold_amt_cp = gold_amt;
    while(gold_amt_cp > 0) {
       str_gold_amt = char(gold_amt_cp % 10 + '0') + str_gold_amt;
       gold_amt_cp /= 10;
    }
    if(str_gold_amt.size() == 0) {
        str_gold_amt = "0";
    }
    string str_iron_amt;
    int iron_amt_cp = iron_amt;
    while(iron_amt_cp > 0) {
       str_iron_amt = char(iron_amt_cp % 10 + '0') + str_iron_amt;
       iron_amt_cp /= 10;
    }
    if(str_iron_amt.size() == 0) {
        str_iron_amt = "0";
    }
    string str_wood_amt;
    int wood_amt_cp = wood_amt;
    while(wood_amt_cp > 0) {
       str_wood_amt = char(wood_amt_cp % 10 + '0') + str_wood_amt;
       wood_amt_cp /= 10;
    }
    if(str_wood_amt.size() == 0) {
        str_wood_amt = "0";
    }
    string str_which;
    switch(which_city_factory) {
        case Player::Me:
            str_which = "Me";
            break;
        case Player::Opponent:
            str_which = "Opponent";
            break;
    }

    return str_which + " Gold:" + str_gold_amt + " Iron:" + str_iron_amt + " Wood:" + str_wood_amt;
}

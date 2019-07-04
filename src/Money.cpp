#include "Money.h"
#include "Enums.h"

using std::string;

std::string Money::Info() {

    string str_wood_amt;
    int wood_amt_cp = wood_amt;
    while(wood_amt_cp > 0) {
        str_wood_amt = char(wood_amt_cp % 10 + '0') + str_wood_amt;
        wood_amt_cp /= 10;
    }
    if(str_wood_amt.size() == 0) {
        str_wood_amt = "0";
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
    string str_gold_amt;
    int gold_amt_cp = gold_amt;
    while(gold_amt_cp > 0) {
        str_gold_amt = char(gold_amt_cp % 10 + '0') + str_gold_amt;
        gold_amt_cp /= 10;
    }
    if(str_gold_amt.size() == 0) {
        str_gold_amt = "0";
    }

    return "Wood:" + str_wood_amt + " Iron:" + str_iron_amt + " Gold:" + str_gold_amt;
}

void Money::Add(int wood_plus, int iron_plus, int gold_plus) {
    gold_amt += gold_plus;
    iron_amt += iron_plus;
    wood_amt += wood_plus;
    return;
}

bool Money::Take(int wood_minus, int iron_minus, int gold_minus) {
    if(gold_minus > gold_amt || iron_minus > iron_amt || wood_minus > wood_amt) {
        return false;
    }
    gold_amt -= gold_minus;
    iron_amt -= iron_minus;
    wood_amt -= wood_minus;
    return true;
}


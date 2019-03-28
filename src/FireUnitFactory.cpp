#include "FireUnitFactory.h"

std::string FireUnitFactory::Info() {
    return UnitFactory::Info() + " Fire";
}

FireUnitFactory::FireUnitFactory(Player which) : UnitFactory(which) {}

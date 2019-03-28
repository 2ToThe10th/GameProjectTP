#include "EarthUnitFactory.h"

std::string EarthUnitFactory::Info() {
    return UnitFactory::Info() + " Earth";
}

EarthUnitFactory::EarthUnitFactory(Player which) : UnitFactory(which) {}

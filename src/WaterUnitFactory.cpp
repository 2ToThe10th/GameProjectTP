#include "WaterUnitFactory.h"

std::string WaterUnitFactory::Info() {
    return UnitFactory::Info() + " Water";
}

WaterUnitFactory::WaterUnitFactory(Player which) : UnitFactory(which) {}

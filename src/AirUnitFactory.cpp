#include "AirUnitFactory.h"

std::string AirUnitFactory::Info() {
    return (UnitFactory::Info() + " Air");
}

AirUnitFactory::AirUnitFactory(Player which) : UnitFactory(which) {}

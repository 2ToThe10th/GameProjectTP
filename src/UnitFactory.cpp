#include "UnitFactory.h"

std::string UnitFactory::Info() {
    switch(which) {
        case Me:
            return "Me";
        case Opponent:
            return "Opponent";
    }
    return "";
}

UnitFactory::UnitFactory(Player which): which(which) {}

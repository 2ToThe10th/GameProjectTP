#include "CreateUnit.h"

CreateUnit::CreateUnit(UnitType unit_type, unsigned int city_id): unit_type(unit_type), city_id(city_id) {}

unsigned int CreateUnit::Do() {
    CityFactory* city_factory = nullptr;
    UnitFactory* unit_factory = nullptr;

    if(which_turn == Player::Me) {
        city_factory = ICommand::my_city_factory;
        unit_factory = ICommand::my_unit_factory;
    }
    else {
        city_factory = ICommand::opponent_city_factory;
        unit_factory = ICommand::opponent_unit_factory;

    }

    if(city_id < 0 || city_id >= city_factory->cities.size()) {
        std::cout << "City with this number does not exist." << std::endl;
        return 1;
    }

    switch (unit_type) {
        case UnitType::WorkerType:
            return unit_factory->AddWorker(city_factory->cities[city_id]);
        case UnitType::ColonistType:
            return unit_factory->AddColonist(city_factory->cities[city_id]);
        case UnitType::WarriorType:
            return unit_factory->AddWarrior(city_factory->cities[city_id]);
        case UnitType::ArcherType:
            return unit_factory->AddArcher(city_factory->cities[city_id]);
        case UnitType::WizardType:
            return unit_factory->AddWizard(city_factory->cities[city_id]);
    }

    return 1;
}

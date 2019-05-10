#include "CreateTower.h"

CreateTower::CreateTower(TowerType tower_type, unsigned int city_id): tower_type(tower_type), city_id(city_id) {}

unsigned int CreateTower::Do() {
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

    if(city_id < 0 || city_id >= city_factory->cities.size() || city_factory->cities[city_id] == nullptr) {
        std::cout << "City with this number does not exist." << std::endl;
        return 1;
    }

    if(tower_type == TowerType::ArcherTower) {
        return city_factory->cities[city_id]->BuildArcherTower(unit_factory->money);
    }
    else {
        return city_factory->cities[city_id]->BuildWizardTower(unit_factory->money);
    }
}



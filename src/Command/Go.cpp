#include "Go.h"

Go::Go(UnitCommonType type, unsigned int unit_id, Direction where):
       type(type), unit_id(unit_id), where(where) {}

unsigned int Go::Do() {
    UnitFactory* unit_factory = nullptr;

    if(which_turn == Player::Me) {
        unit_factory = ICommand::my_unit_factory;
    }
    else {
        unit_factory = ICommand::opponent_unit_factory;
    }

    if(unit_id < 0) {
        std::cout << "Unit with this number does not exist." << std::endl;
        return 1;
    }

    switch (type) {
        case UnitCommonType::ColonistCommonType:
            if(unit_factory->list_colonist.size() <= unit_id || unit_factory->list_colonist[unit_id] == nullptr){
                std::cout << "Unit with this number does not exist." << std::endl;
                return 1;
            }
            return unit_factory->list_colonist[unit_id]->Go(where);
        case UnitCommonType::CombatCommonType:
            if(unit_factory->list_combat_unit.size() <= unit_id || unit_factory->list_combat_unit[unit_id] == nullptr){
                std::cout << "Unit with this number does not exist." << std::endl;
                return 1;
            }
            return unit_factory->list_combat_unit[unit_id]->Go(where);
        case UnitCommonType::WorkerCommonType:
            if(unit_factory->list_worker.size() <= unit_id || unit_factory->list_worker[unit_id] == nullptr){
                std::cout << "Unit with this number does not exist." << std::endl;
                return 1;
            }
            return unit_factory->list_worker[unit_id]->Go(where);
    }

    return 1;
}

void Go::Send() {
    string string_to_send = "go ";
    switch (type) {
        case UnitCommonType::WorkerCommonType:
            string_to_send += "worker";
            break;
        case UnitCommonType::ColonistCommonType:
            string_to_send += "colonist";
            break;
        case UnitCommonType::CombatCommonType:
            string_to_send += "combat";
            break;
    }
    string_to_send += " " + std::to_string(unit_id) + " ";
    switch (where) {
        case Direction::Up:
            string_to_send.push_back('u');
            break;
        case Direction::Down:
            string_to_send.push_back('d');
            break;
        case Direction::Left:
            string_to_send.push_back('l');
            break;
        case Direction::Right:
            string_to_send.push_back('r');
            break;
    }
    socket->Write(string_to_send);
}


#include "Attack.h"

Attack::Attack(unsigned int combat_id, std::vector<Direction>& where): combat_id(combat_id), where(where.size()) {
    for(unsigned int i = 0; i < where.size(); ++i) {
        this->where[i] = where[i];
    }
}

unsigned int Attack::Do() {
    UnitFactory* unit_factory = nullptr;

    if(which_turn == Player::Me) {
        unit_factory = ICommand::my_unit_factory;
    }
    else {
        unit_factory = ICommand::opponent_unit_factory;
    }

    if(combat_id < 0 || combat_id >= unit_factory->list_combat_unit.size() ||
    unit_factory->list_combat_unit[combat_id] == nullptr) {
        std::cout << "Combat unit with this number does not exist." << std::endl;
        return 1;
    }

    return unit_factory->list_combat_unit[combat_id]->Attack(where);
}

void Attack::Send() {
    string string_to_send = "attack " + std::to_string(combat_id) + " ";
    for(auto i: where) {
        switch (i) {
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
    }
    socket->Write(string_to_send);
}

#include "Mine.h"

Mine::Mine(unsigned int worker_id): worker_id(worker_id) {}

unsigned int Mine::Do() {
    UnitFactory* unit_factory = nullptr;

    if(which_turn == Player::Me) {
        unit_factory = ICommand::my_unit_factory;
    }
    else {
        unit_factory = ICommand::opponent_unit_factory;
    }

    if(worker_id < 0 || worker_id >= unit_factory->list_worker.size() || unit_factory->list_worker[worker_id] == nullptr) {
        std::cout << "Worker with this number does not exist" << std::endl;
        return 1;
    }

    return unit_factory->list_worker[worker_id]->Mine();
}

void Mine::Send() {
    string string_to_send = "mine " + std::to_string(worker_id);
    socket->Write(string_to_send);
}

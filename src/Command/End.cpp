#include "End.h"

unsigned int End::Do() {
    ICommand::NewTurn();

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

    int first_not_nullptr = -1;
    for(int i = city_factory->cities.size() - 1; i >= 0; --i) {
        if(city_factory->cities[i] != nullptr){
            first_not_nullptr = i;
            break;
        }
    }

    if(first_not_nullptr == -1) {
        if(which_turn == Player::Me) {
            std::cout << "You lose" << std::endl;
        }
        else{
            std::cout << "You win!!!!" << std::endl;
        }
        return 1;
    }

    city_factory->cities.resize(first_not_nullptr + 1);

    for(auto combat_unit: unit_factory->list_combat_unit) {
        if(combat_unit != nullptr) {
            combat_unit->NewTurn();
        }
    }

    for(auto worker: unit_factory->list_worker) {
        if (worker != nullptr) {
            worker->NewTurn();
        }
    }

    for(auto colonist: unit_factory->list_colonist) {
        if(colonist != nullptr) {
            colonist->NewTurn();
        }
    }

    return 0;
}

End::End() {
    ICommand::is_end = true;
}

void End::Send() {
    string string_to_send = "end ";
    socket->Write(string_to_send);
}

#include "ICommand.h"

GameSocket* ICommand::socket = nullptr;
UnitFactory* ICommand::my_unit_factory = nullptr;
UnitFactory* ICommand::opponent_unit_factory = nullptr;
CityFactory* ICommand::my_city_factory = nullptr;
CityFactory* ICommand::opponent_ciy_factory = nullptr;

Player ICommand::which_turn = Player::Me;

void ICommand::NewTurn() {
    if(which_turn == Player::Me) {
        which_turn = Player::Opponent;
    }
    else {
        which_turn = Player::Me;
    }
}

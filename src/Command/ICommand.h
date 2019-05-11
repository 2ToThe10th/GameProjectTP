#ifndef GAME_ICOMMAND_H
#define GAME_ICOMMAND_H

#include "GameSocket.h"
#include "UnitFactory.h"

class ICommand {
public:
    static GameSocket* socket;
    static UnitFactory* my_unit_factory;
    static UnitFactory* opponent_unit_factory;
    static CityFactory* my_city_factory;
    static CityFactory* opponent_city_factory;

    static bool is_end;
    static Player which_turn;

    virtual ~ICommand() = default;

    virtual unsigned int Do() = 0;
    virtual void Send() = 0;
    static void NewTurn();
};

#endif //GAME_ICOMMAND_H

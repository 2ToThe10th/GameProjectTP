#ifndef GAME_WRONGCOMMAND_H
#define GAME_WRONGCOMMAND_H


#include "ICommand.h"

class WrongCommand: public ICommand {
public:
    WrongCommand() = default;

    unsigned int Do() override;
    void Send() override {};
};


#endif //GAME_WRONGCOMMAND_H

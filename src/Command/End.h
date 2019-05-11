#ifndef GAME_END_H
#define GAME_END_H


#include "ICommand.h"

class End: public ICommand {
public:
    End();

    unsigned int Do() override;
};


#endif //GAME_END_H

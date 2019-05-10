#ifndef GAME_MINE_H
#define GAME_MINE_H


#include "ICommand.h"

class Mine: public ICommand {
private:
    unsigned int worker_id;
public:
    Mine(unsigned int worker_id);

    unsigned int Do() override;
};


#endif //GAME_MINE_H

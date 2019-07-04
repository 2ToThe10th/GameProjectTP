#ifndef GAME_ATTACK_H
#define GAME_ATTACK_H


#include "ICommand.h"

class Attack: public ICommand {
private:
    unsigned int combat_id;
    std::vector<Direction> where;
public:
    Attack(unsigned int combat_id, std::vector<Direction>& where);

    unsigned int Do() override;
    void Send() override;
};


#endif //GAME_ATTACK_H

#ifndef GAME_CREATETOWER_H
#define GAME_CREATETOWER_H

#include "Enums.h"
#include "ICommand.h"

class CreateTower: public ICommand {
private:
    TowerType tower_type;
    unsigned int city_id;
public:
    CreateTower(TowerType tower_type, unsigned int city_id);

    unsigned int Do() override;
    void Send() override;
};


#endif //GAME_CREATETOWER_H

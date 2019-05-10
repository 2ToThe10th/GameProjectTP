#ifndef GAME_CREATEUNIT_H
#define GAME_CREATEUNIT_H

#include "Enums.h"
#include "ICommand.h"

class CreateUnit: public ICommand {
private:
    UnitType unit_type;
    unsigned int city_id;
public:
    CreateUnit(UnitType unit_type, unsigned int city_id);

    unsigned int Do() override;
};


#endif //GAME_CREATEUNIT_H

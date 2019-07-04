#ifndef GAME_GO_H
#define GAME_GO_H


#include <vector>
#include "Enums.h"
#include "ICommand.h"

class Go: public ICommand {
private:
    UnitCommonType type;
    unsigned int unit_id;
    Direction where;
public:
    Go(UnitCommonType type, unsigned int unit_id, Direction where);

    unsigned int Do() override;
    void Send() override;
};


#endif //GAME_GO_H

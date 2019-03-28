#ifndef GAME_COLONIST_H
#define GAME_COLONIST_H

#include "Enums.h"
#include "Unit.h"

class Colonist: public Unit {
    ~Colonist();

    void Info();
    void Go(Direction where);
};


#endif //GAME_COLONIST_H

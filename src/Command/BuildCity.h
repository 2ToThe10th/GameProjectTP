#ifndef GAME_BUILDCITY_H
#define GAME_BUILDCITY_H

#include "ICommand.h"

class BuildCity: public ICommand {
private:
    unsigned int colonist_id;
public:
    BuildCity(unsigned int colonist_id);

    unsigned int Do() override;
};

#endif //GAME_BUILDCITY_H

#ifndef GAME_EARTHUNITFACTORY_H
#define GAME_EARTHUNITFACTORY_H


#include "UnitFactory.h"

class EarthUnitFactory: public UnitFactory {
private:
public:
    EarthUnitFactory(Player which, Map& map, Money& money);
    ~EarthUnitFactory() override = default;

    void AddWarrior(City& where) override;
    void AddArcher(City& where) override;
    void AddWizard(City& where) override;

    std::string Info() override;
};


#endif //GAME_EARTHUNITFACTORY_H

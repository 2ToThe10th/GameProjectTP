#ifndef GAME_AIRUNITFACTORY_H
#define GAME_AIRUNITFACTORY_H


#include "UnitFactory.h"

class AirUnitFactory: public UnitFactory {
private:
public:
    AirUnitFactory(Player which, Map& map, Money& money);
    ~AirUnitFactory() override = default;

    void AddWarrior(City& where) override;
    void AddArcher(City& where) override;
    void AddWizard(City& where) override;

    std::string Info() override;
};


#endif //GAME_AIRUNITFACTORY_H

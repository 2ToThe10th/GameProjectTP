#ifndef GAME_WATERUNITFACTORY_H
#define GAME_WATERUNITFACTORY_H


#include "UnitFactory.h"

class WaterUnitFactory: public UnitFactory {
private:
public:
    WaterUnitFactory(Player which, Map& map, Money& money);
    ~WaterUnitFactory() override = default;

    void AddWarrior(City& where) override;
    void AddArcher(City& where) override;
    void AddWizard(City& where) override;

    std::string Info() override;
};


#endif //GAME_WATERUNITFACTORY_H

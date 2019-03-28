#ifndef GAME_WATERUNITFACTORY_H
#define GAME_WATERUNITFACTORY_H


#include "UnitFactory.h"

class WaterUnitFactory: public UnitFactory {
private:
public:
    WaterUnitFactory(Player which);
    ~WaterUnitFactory() override {};

    void AddWarrior(City where) override {};
    void AddArcher(City where) override {};
    void AddWizard(City where) override {};
    void AddColonist(City where) override {};
    void AddWorker(City where) override {};

    std::string Info() override;
};


#endif //GAME_WATERUNITFACTORY_H

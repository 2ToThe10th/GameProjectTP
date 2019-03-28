#ifndef GAME_AIRUNITFACTORY_H
#define GAME_AIRUNITFACTORY_H


#include "UnitFactory.h"

class AirUnitFactory: public UnitFactory {
private:
public:
    AirUnitFactory(Player which);
    ~AirUnitFactory() override {};

    void AddWarrior(City where) override {};
    void AddArcher(City where) override {};
    void AddWizard(City where) override {};
    void AddColonist(City where) override {};
    void AddWorker(City where) override {};

    std::string Info() override;
};


#endif //GAME_AIRUNITFACTORY_H

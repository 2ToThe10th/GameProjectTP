#ifndef GAME_FIREUNITFACTORY_H
#define GAME_FIREUNITFACTORY_H


#include "UnitFactory.h"

class FireUnitFactory: public UnitFactory {
private:
public:
    FireUnitFactory(Player which);
    ~FireUnitFactory() override {};

    void AddWarrior(City where) override {};
    void AddArcher(City where) override {};
    void AddWizard(City where) override {};
    void AddColonist(City where) override {};
    void AddWorker(City where) override {};

    std::string Info() override;
};


#endif //GAME_FIREUNITFACTORY_H

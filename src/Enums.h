#ifndef GAME_ENUMS_H
#define GAME_ENUMS_H

enum Player {
    Me,
    Opponent
};

enum Direction {
    Up,
    Left,
    Right,
    Down
};

enum Race {
    Water,
    Fire,
    Earth,
    Air
};

enum Resource {
    Nothing,
    Gold,
    Silver,
    Wood
};

enum UnitType {
    WorkerType,
    ColonistType,
    WarriorType,
    ArcherType,
    WizardType
};

enum TowerType {
    ArcherTower,
    WizardTower
};

enum UnitCommonType {
    WorkerCommonType,
    ColonistCommonType,
    CombatCommonType
};

enum CombatUnitType {
    WarriorCombatType,
    ArcherCombatType,
    WizardCombatType
};

#endif //GAME_ENUMS_H

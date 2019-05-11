#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <CityFactory.h>
#include <Unit.h>
#include <Command/ICommand.h>
#include <Command/BuildCity.h>
#include <Command/CreateUnit.h>
#include <Command/CreateTower.h>
#include <Command/End.h>
#include <Command/Mine.h>
#include <Command/Go.h>
#include <Command/Attack.h>
#include "Location.h"
#include "Map.h"
#include "Money.h"
#include "UnitFactory.h"
#include "GameSocket.h"
#include "Warrior.h"

using std::cin;
using std::cout;
using std::endl;
using std::to_string;
using std::vector;

/*
TEST(GameSocket, server) {
    string s;
    getline(cin, s);
    s += '\n';

    GameSocket* sd = new GameSocket();

    sd->Write(s);
    cout << sd->Read();

    delete sd;
}

TEST(GameSocket, client) {

    string s;
    getline(cin, s);
    s += '\n';

    GameSocket* sd = new GameSocket("127.0.0.1");


    sd->Write(s);
    cout << sd->Read();

    delete sd;
}
*/

TEST(Location, Direction) {

    Location place(4,6);

    ASSERT_EQ(place.Direction(Direction::Up), Location(5, 6));
    ASSERT_EQ(place.Direction(Direction::Right), Location(4, 7));
    ASSERT_EQ(place.Direction(Direction::Left), Location(4, 5));
    ASSERT_EQ(place.Direction(Direction::Down), Location(3, 6));

}

TEST(Map, Constructor) {
    auto map = new Map();

    const int MAX_HEIGHT = 20;
    const int MAX_WIDTH = 20;

    string sample;
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    EXPECT_EQ(map->Info(), sample);

    EXPECT_EQ(map->colonist(Location(3,4)), nullptr);
    EXPECT_EQ(map->combat(Location(5,6)), nullptr);
    EXPECT_EQ(map->worker(Location(7,8)), nullptr);

    delete map;
}

TEST(Colonist, Delete) {
    auto map = new Map();
    Money money;
    money.Add(100500, 100500, 100500);
    auto factory = new UnitFactory(Player::Me, map, money, Race::Fire);
    auto city = new City(Location(2,2));
    factory->AddColonist(city);
    Colonist* colonist = factory->list_colonist[0];

    Unit::my_unit_factory = factory;

    delete colonist;

    delete map;
    delete factory;
    delete city;
}

TEST(CityFactory, AddCity) {
    auto map = new Map();
    Money money;
    money.Add(100500, 100500, 100500);
    auto factory = new UnitFactory(Player::Me, map, money, Race::Fire);
    Unit::my_unit_factory = factory;
    auto city = new City(Location(2,2));
    factory->AddColonist(city);
    delete city;
    Colonist* colonist = factory->list_colonist[0];
    auto city_factory = new CityFactory(Player::Me, map);

    Unit::my_unit_factory = factory;

    EXPECT_EQ(city_factory->cities.size(), 0);

    city_factory->AddCity(colonist);
    EXPECT_EQ(city_factory->cities.size(), 1);

    city = new City(Location(2,2));
    factory->AddColonist(city);
    delete city;
    colonist = factory->list_colonist[0];

    city_factory->AddCity(colonist);
    EXPECT_EQ(city_factory->cities.size(), 1);

    city_factory->AddCity(nullptr);
    EXPECT_EQ(city_factory->cities.size(), 1);

    delete city_factory;
    delete factory;
    delete map;
}

TEST(Money, Add_Take) {
    Money money;

    EXPECT_EQ(money.Info(), "Wood:100500 Iron:100500 Gold:100500");

    EXPECT_EQ(money.Take(1, 2, 3), true);

    EXPECT_EQ(money.Info(), "Wood:100499 Iron:100498 Gold:100497");

    EXPECT_EQ(money.Take(99999, 99998, 99997), true);

    EXPECT_EQ(money.Info(), "Wood:500 Iron:500 Gold:500");

    EXPECT_EQ(money.Take(1, 2, 999), false);
    EXPECT_EQ(money.Take(1, 2999, 9), false);
    EXPECT_EQ(money.Take(1999, 2, 99), false);

    EXPECT_EQ(money.Info(), "Wood:500 Iron:500 Gold:500");

    EXPECT_EQ(money.Take(500, 500, 500), true);

    EXPECT_EQ(money.Info(), "Wood:0 Iron:0 Gold:0");

    money.Add(1, 2, 3);

    EXPECT_EQ(money.Info(), "Wood:1 Iron:2 Gold:3");

    EXPECT_EQ(money.Take(1, 2, 3), true);

    EXPECT_EQ(money.Info(), "Wood:0 Iron:0 Gold:0");
}

TEST(Colonist, Go) {
    Money money;
    money.Add(1150, 1050 , 100);
    auto map = new Map();
    auto factory = new UnitFactory(Player::Me, map, money, Race::Earth);

    Location location(0, 5);

    auto city = new City(location);
    factory->AddColonist(city);
    delete city;
    EXPECT_NE(map->colonist(location), nullptr);

    auto colonist = map->colonist(location);

    colonist->NewTurn();
    colonist->Go(Direction::Up);
    EXPECT_EQ(map->colonist(location), nullptr);
    location = location.Direction(Direction::Up);
    EXPECT_EQ(map->colonist(location), colonist);

    colonist->NewTurn();
    colonist->Go(Direction::Left);
    EXPECT_EQ(map->colonist(location), nullptr);
    location = location.Direction(Direction::Left);
    EXPECT_EQ(map->colonist(location), colonist);

    colonist->NewTurn();
    colonist->Go(Direction::Right);
    EXPECT_EQ(map->colonist(location), nullptr);
    location = location.Direction(Direction::Right);
    EXPECT_EQ(map->colonist(location), colonist);

    colonist->NewTurn();
    colonist->Go(Direction::Down);
    EXPECT_EQ(map->colonist(location), nullptr);
    location = location.Direction(Direction::Down);
    EXPECT_EQ(map->colonist(location), colonist);

    colonist->NewTurn();
    colonist->Go(Direction::Down);
    EXPECT_EQ(map->colonist(location), colonist);

    city = new City(location.Direction(Direction::Up));
    factory->AddWarrior(city);
    delete city;

    colonist->NewTurn();
    colonist->Go(Direction::Up);
    EXPECT_EQ(map->colonist(location), nullptr);
    location = location.Direction(Direction::Up);
    EXPECT_EQ(map->colonist(location), colonist);

    auto enemy_factory = new UnitFactory(Player::Opponent, map, money, Race::Water);

    city = new City(location.Direction(Direction::Up));
    enemy_factory->AddWarrior(city);
    delete city;

    colonist->NewTurn();
    colonist->Go(Direction::Up);
    EXPECT_EQ(map->colonist(location), colonist);
    location = location.Direction(Direction::Up);
    EXPECT_EQ(map->colonist(location), nullptr);

    Unit::my_unit_factory = factory;
    Unit::opponent_unit_factory = enemy_factory;

    delete factory;
    delete enemy_factory;
    delete map;
}

TEST(Worker, Go) {
    Money money;
    money.Add(1050, 1010 , 100);
    auto map = new Map();
    auto factory = new UnitFactory(Player::Me, map, money, Race::Air);

    Unit::my_unit_factory = factory;

    Location location(0, 7);

    auto city = new City(location);
    factory->AddWorker(city);
    delete city;
    EXPECT_NE(map->worker(location), nullptr);

    auto worker = map->worker(location);

    worker->NewTurn();
    worker->Go(Direction::Up);
    EXPECT_EQ(map->worker(location), nullptr);
    location = location.Direction(Direction::Up);
    EXPECT_EQ(map->worker(location), worker);

    worker->NewTurn();
    worker->Go(Direction::Left);
    EXPECT_EQ(map->worker(location), nullptr);
    location = location.Direction(Direction::Left);
    EXPECT_EQ(map->worker(location), worker);

    worker->NewTurn();
    worker->Go(Direction::Right);
    EXPECT_EQ(map->worker(location), nullptr);
    location = location.Direction(Direction::Right);
    EXPECT_EQ(map->worker(location), worker);

    worker->NewTurn();
    worker->Go(Direction::Down);
    EXPECT_EQ(map->worker(location), nullptr);
    location = location.Direction(Direction::Down);
    EXPECT_EQ(map->worker(location), worker);

    worker->NewTurn();
    worker->Go(Direction::Down);
    EXPECT_EQ(map->worker(location), worker);

    city = new City(location.Direction(Direction::Up));
    factory->AddWarrior(city);
    delete city;

    worker->NewTurn();
    worker->Go(Direction::Up);
    EXPECT_EQ(map->worker(location), nullptr);
    location = location.Direction(Direction::Up);
    EXPECT_EQ(map->worker(location), worker);

    auto enemy_factory = new UnitFactory(Player::Opponent, map, money, Race::Earth);
    Unit::opponent_unit_factory = enemy_factory;

    city = new City(location.Direction(Direction::Up));
    enemy_factory->AddWarrior(city);
    delete city;

    worker->NewTurn();
    worker->Go(Direction::Up);
    EXPECT_EQ(map->worker(location), worker);
    location = location.Direction(Direction::Up);
    EXPECT_EQ(map->worker(location), nullptr);

    delete enemy_factory;
    delete factory;
    delete map;
}

TEST(CombatUnit, Go) {
    Money money;
    money.Add(11700, 11500 , 12100);
    auto map = new Map();
    auto factory = new UnitFactory(Player::Me, map, money, Race::Fire);
    Unit::my_unit_factory = factory;

    Location location(0, 4);

    auto city = new City(location);

    city->BuildArcherTower(money);
    city->BuildWizardTower(money);

    factory->AddWizard(city);
    EXPECT_NE(map->combat(location), nullptr);
    delete city;

    auto combat = map->combat(location);

    combat->NewTurn();
    combat->Go(Direction::Up);
    EXPECT_EQ(map->combat(location), nullptr);
    location = location.Direction(Direction::Up);
    EXPECT_EQ(map->combat(location), combat);

    combat->NewTurn();
    combat->Go(Direction::Left);
    EXPECT_EQ(map->combat(location), nullptr);
    location = location.Direction(Direction::Left);
    EXPECT_EQ(map->combat(location), combat);

    combat->NewTurn();
    combat->Go(Direction::Right);
    EXPECT_EQ(map->combat(location), nullptr);
    location = location.Direction(Direction::Right);
    EXPECT_EQ(map->combat(location), combat);

    combat->NewTurn();
    combat->Go(Direction::Down);
    EXPECT_EQ(map->combat(location), nullptr);
    location = location.Direction(Direction::Down);
    EXPECT_EQ(map->combat(location), combat);

    combat->NewTurn();
    combat->Go(Direction::Down);
    EXPECT_EQ(map->combat(location), combat);

    auto enemy_factory = new UnitFactory(Player::Opponent, map, money, Race::Air);
    Unit::opponent_unit_factory = enemy_factory;
    city = new City(location.Direction(Direction::Up));
    enemy_factory->AddColonist(city);
    delete city;

    combat->NewTurn();
    combat->Go(Direction::Up);
    EXPECT_EQ(map->combat(location), nullptr);
    location = location.Direction(Direction::Up);
    EXPECT_EQ(map->combat(location), combat);

    EXPECT_EQ(enemy_factory->list_colonist.size(), 1);
    EXPECT_EQ(enemy_factory->list_colonist[0], nullptr);
    EXPECT_EQ(factory->list_colonist.size(), 1);
    EXPECT_EQ(factory->list_colonist[0]->which, Player::Me);
    
    Location second_location = location.Direction(Direction::Up).Direction(Direction::Right);

    city = new City(second_location);
    enemy_factory->AddWarrior(city);
    delete city;
    city = new City(Location(12, 13));
    factory->AddWorker(city);
    delete city;
    city = new City(second_location.Direction(Direction::Left));
    factory->AddWorker(city);
    delete city;

    auto second_combat = map->combat(second_location);
    
    second_combat->NewTurn();
    second_combat->Go(Direction::Left);
    EXPECT_EQ(map->combat(second_location), nullptr);
    second_location = second_location.Direction(Direction::Left);
    EXPECT_EQ(map->combat(second_location), second_combat);

    EXPECT_EQ(factory->list_worker.size(), 2);
    EXPECT_EQ(factory->list_worker[1], nullptr);
    EXPECT_EQ(enemy_factory->list_worker.size(), 1);
    EXPECT_EQ(enemy_factory->list_worker[0]->which, Player::Opponent);

    second_combat->NewTurn();
    second_combat->Go(Direction::Right);
    EXPECT_EQ(map->combat(second_location), nullptr);
    second_location = second_location.Direction(Direction::Right);
    EXPECT_EQ(map->combat(second_location), second_combat);

    combat->NewTurn();
    combat->Go(Direction::Up);
    EXPECT_EQ(map->combat(location), nullptr);
    location = location.Direction(Direction::Up);
    EXPECT_EQ(map->combat(location), combat);

    EXPECT_EQ(enemy_factory->list_worker.size(), 1);
    EXPECT_EQ(enemy_factory->list_worker[0], nullptr);
    EXPECT_EQ(factory->list_worker.size(), 2);
    EXPECT_EQ(factory->list_worker[1]->which, Player::Me);

    delete factory;
    delete enemy_factory;
    delete map;
}

TEST(CombatUnit, AttackCity) {
    Money money;
    money.Add(11700, 11500 , 12100);
    auto map = new Map();
    auto factory = new UnitFactory(Player::Me, map, money, Race::Fire);
    Unit::my_unit_factory = factory;
    auto enemy_factory = new UnitFactory(Player::Opponent, map, money, Race::Air);
    Unit::opponent_unit_factory = enemy_factory;
    auto my_city_factory = new CityFactory(Player::Me, map);
    City::my_city_factory = my_city_factory;
    auto opponent_city_factory = new CityFactory(Player::Opponent, map);
    City::opponent_city_factory = opponent_city_factory;

    Location my_city_location(3, 14);

    auto city = new City(my_city_location);
    factory->AddColonist(city);
    delete city;
    my_city_factory->AddCity(factory->list_colonist[0]);
    EXPECT_EQ(factory->list_colonist[0], nullptr);
    auto my_city = my_city_factory->cities[0];
    my_city->BuildArcherTower(money);
    my_city->BuildWizardTower(money);
    factory->AddWizard(my_city);

    auto my_wizard = factory->list_combat_unit[0];

    Location opponent_city_location = my_city_location.Direction(Direction::Up).Direction(Direction::Right);

    city = new City(opponent_city_location);
    enemy_factory->AddColonist(city);
    delete city;
    opponent_city_factory->AddCity(enemy_factory->list_colonist[0]);
    EXPECT_EQ(enemy_factory->list_colonist[0], nullptr);
    auto opponent_city = opponent_city_factory->cities[0];
    opponent_city->BuildArcherTower(money);
    opponent_city->BuildWizardTower(money);
    enemy_factory->AddWizard(opponent_city);

    auto enemy_wizard = enemy_factory->list_combat_unit[0];

    enemy_wizard->NewTurn();
    enemy_wizard->Go(Direction::Down);
    my_wizard->NewTurn();
    my_wizard->Go(Direction::Up);
    my_wizard->NewTurn();
    my_wizard->Go(Direction::Right);

    EXPECT_EQ(my_city_factory->cities.size(), 2);
    EXPECT_EQ(my_city_factory->cities[0], my_city);
    EXPECT_EQ(my_city_factory->cities[1], opponent_city);
    EXPECT_EQ(opponent_city_factory->cities.size(), 1);
    EXPECT_EQ(opponent_city_factory->cities[0], nullptr);

    enemy_wizard->NewTurn();
    enemy_wizard->Go(Direction::Left);

    EXPECT_EQ(my_city_factory->cities.size(), 2);
    EXPECT_EQ(my_city_factory->cities[0], nullptr);
    EXPECT_EQ(my_city_factory->cities[1], opponent_city);
    EXPECT_EQ(opponent_city_factory->cities.size(), 1);
    EXPECT_EQ(opponent_city_factory->cities[0], my_city);

    delete factory;
    delete enemy_factory;
    delete my_city_factory;
    delete opponent_city_factory;
    delete map;
}

TEST(CombatUnit, Die) {
    Money money;
    money.Add(1700, 1500 , 2100);
    auto map = new Map();
    auto my_factory = new UnitFactory(Player::Me, map, money, Race::Earth);

    Unit::my_unit_factory = my_factory;

    Location my_warrior_location(2, 3);

    auto city = new City(my_warrior_location);
    my_factory->AddWarrior(city);
    delete city;

    delete my_factory->list_combat_unit[0];

    EXPECT_EQ(my_factory->list_combat_unit[0], nullptr);

    delete my_factory;
    delete map;
}

TEST(Warrior, Attack) {
    Money money;
    money.Add(1700, 1500 , 2100);
    auto map = new Map();
    auto my_factory = new UnitFactory(Player::Me, map, money, Race::Earth);
    Unit::my_unit_factory = my_factory;
    auto enemy_factory = new UnitFactory(Player::Opponent, map, money, Race::Fire);
    Unit::opponent_unit_factory = enemy_factory;

    Location my_warrior_location(2, 3);

    auto city = new City(my_warrior_location);
    my_factory->AddWarrior(city);
    delete city;

    auto my_warrior = my_factory->list_combat_unit[0];

    Location enemy_warrior_location = my_warrior_location.Direction(Direction::Left);

    city = new City(enemy_warrior_location);
    enemy_factory->AddWarrior(city);
    delete city;

    auto enemy_warrior = enemy_factory->list_combat_unit[0];

    my_warrior->NewTurn();

    vector<Direction> vec;

    my_warrior->Attack(vec);
    EXPECT_EQ(my_warrior->health, my_warrior->MAX_HEALTH);

    vec.push_back(Direction::Down);
    my_warrior->Attack(vec);
    EXPECT_EQ(my_warrior->health, my_warrior->MAX_HEALTH);

    vec[0] = Direction::Left;
    my_warrior->Attack(vec);
    EXPECT_EQ(my_warrior->health, my_warrior->MAX_HEALTH - enemy_warrior->damage);
    EXPECT_EQ(enemy_warrior->health, enemy_warrior->MAX_HEALTH - my_warrior->damage);

    enemy_warrior->health = my_warrior->damage;

    my_warrior->Attack(vec);
    EXPECT_EQ(my_warrior->health, my_warrior->MAX_HEALTH - enemy_warrior->damage);

    my_warrior->NewTurn();
    int enemy_damage = enemy_warrior->damage;
    my_warrior->Attack(vec);
    EXPECT_EQ(my_warrior->health, my_warrior->MAX_HEALTH - 2*enemy_damage);
    EXPECT_EQ(map->combat(my_warrior_location), nullptr);
    EXPECT_EQ(map->combat(enemy_warrior_location), my_warrior);

    my_warrior_location = enemy_warrior_location;
    enemy_warrior_location = enemy_warrior_location.Direction(Direction::Down);

    city = new City(enemy_warrior_location);
    enemy_factory->AddWarrior(city);
    delete city;

    enemy_warrior = enemy_factory->list_combat_unit[0];
    EXPECT_NE(enemy_warrior, nullptr);

    vec.pop_back();
    enemy_warrior->NewTurn();
    enemy_warrior->Attack(vec);
    EXPECT_EQ(my_warrior->health, my_warrior->MAX_HEALTH - 2*enemy_damage);
    EXPECT_EQ(enemy_warrior->health, enemy_warrior->MAX_HEALTH);

    vec.push_back(Direction::Down);
    enemy_warrior->Attack(vec);
    EXPECT_EQ(my_warrior->health, my_warrior->MAX_HEALTH - 2*enemy_damage);
    EXPECT_EQ(enemy_warrior->health, enemy_warrior->MAX_HEALTH);

    int enemy_attack = 2 * enemy_damage;
    vec[0] = Direction::Up;

    while(my_warrior->health > enemy_warrior->damage) {
        enemy_warrior->Attack(vec);
        enemy_attack += enemy_warrior->damage;
        EXPECT_EQ(my_warrior->health, my_warrior->MAX_HEALTH - enemy_attack);
        enemy_warrior->NewTurn();
    }

    enemy_warrior->Attack(vec);

    EXPECT_EQ(my_factory->list_combat_unit[0], nullptr);
    EXPECT_EQ(map->combat(enemy_warrior_location), nullptr);
    EXPECT_EQ(map->combat(my_warrior_location), enemy_warrior);

    delete my_factory;
    delete enemy_factory;
    delete map;
}

TEST(Archer, Attack) {
    Money money;
    money.Add(1700, 1500 , 2100);
    auto map = new Map();
    auto my_factory = new UnitFactory(Player::Me, map, money, Race::Fire);
    Unit::my_unit_factory = my_factory;
    auto enemy_factory = new UnitFactory(Player::Opponent, map, money, Race::Air);
    Unit::opponent_unit_factory = enemy_factory;

    Location my_unit_location(5, 6);

    auto city = new City(my_unit_location);
    city->BuildArcherTower(money);
    my_factory->AddArcher(city);
    delete city;

    auto my_archer = my_factory->list_combat_unit[0];

    Location opponent_unit_location(6,7);

    city = new City(opponent_unit_location);
    enemy_factory->AddWarrior(city);
    delete city;

    auto enemy_warrior = enemy_factory->list_combat_unit[0];
    vector<Direction> vec = {Direction::Up, Direction::Right};
    my_archer->NewTurn();
    my_archer->Attack(vec);
    EXPECT_EQ(enemy_warrior->health, enemy_warrior->MAX_HEALTH - my_archer->damage);

    my_archer->Attack(vec);
    EXPECT_EQ(enemy_warrior->health, enemy_warrior->MAX_HEALTH - my_archer->damage);

    my_archer->NewTurn();
    vec.push_back(Direction::Up);
    my_archer->Attack(vec);
    EXPECT_EQ(enemy_warrior->health, enemy_warrior->MAX_HEALTH - my_archer->damage);
    vec.pop_back();

    enemy_warrior->NewTurn();
    enemy_warrior->Go(Direction::Left);
    EXPECT_EQ(map->combat(opponent_unit_location), nullptr);
    opponent_unit_location = opponent_unit_location.Direction(Direction::Left);
    EXPECT_EQ(map->combat(opponent_unit_location), enemy_warrior);

    vec.pop_back();

    my_archer->Attack(vec);
    EXPECT_EQ(map->combat(opponent_unit_location), nullptr);

    opponent_unit_location = opponent_unit_location.Direction(Direction::Left);
    city = new City(opponent_unit_location);
    enemy_factory->AddWorker(city);
    delete city;

    vec.push_back(Direction::Left);

    my_archer->Attack(vec);
    EXPECT_NE(map->worker(opponent_unit_location), nullptr);

    my_archer->NewTurn();
    my_archer->Attack(vec);
    EXPECT_EQ(map->worker(opponent_unit_location), nullptr);

    delete my_factory;
    delete enemy_factory;
    delete map;
}

void create_9_unit(UnitFactory* enemy_factory, Location location_for_enemy) {
    auto city = new City(location_for_enemy);
    enemy_factory->AddWarrior(city);
    delete city;
    city = new City(location_for_enemy.Direction(Direction::Right));
    enemy_factory->AddWarrior(city);
    delete city;
    city = new City(location_for_enemy.Direction(Direction::Left));
    enemy_factory->AddWarrior(city);
    delete city;
    city = new City(location_for_enemy.Direction(Direction::Up));
    enemy_factory->AddWarrior(city);
    delete city;
    city = new City(location_for_enemy.Direction(Direction::Down));
    enemy_factory->AddWarrior(city);
    delete city;
    city = new City(location_for_enemy.Direction(Direction::Right).Direction(Direction::Up));
    enemy_factory->AddWarrior(city);
    delete city;
    city = new City(location_for_enemy.Direction(Direction::Right).Direction(Direction::Down));
    enemy_factory->AddWarrior(city);
    delete city;
    city = new City(location_for_enemy.Direction(Direction::Left).Direction(Direction::Up));
    enemy_factory->AddWarrior(city);
    delete city;
    city = new City(location_for_enemy.Direction(Direction::Left).Direction(Direction::Down));
    enemy_factory->AddWarrior(city);
    delete city;
}

TEST(WaterWizard, Attack) {
    Money money;
    money.Add(11700, 11500 , 12100);
    auto map = new Map();
    auto enemy_factory = new UnitFactory(Player::Opponent, map, money, Race::Earth);
    Unit::opponent_unit_factory = enemy_factory;

    Location location_for_enemy = Location(4, 5);

    create_9_unit(enemy_factory, location_for_enemy);

    auto my_factory = new UnitFactory(Player::Me, map, money, Race::Water);
    Unit::my_unit_factory = my_factory;

    Location location_for_wizard = location_for_enemy.Direction(Direction::Up).Direction(Direction::Up).Direction(Direction::Right);

    auto city = new City(location_for_wizard);
    city->BuildArcherTower(money);
    city->BuildWizardTower(money);
    my_factory->AddWizard(city);
    delete city;

    auto my_wizard = my_factory->list_combat_unit[0];

    my_wizard->NewTurn();
    vector<Direction> to_attack = {Direction::Down, Direction::Down, Direction::Left};
    my_wizard->Attack(to_attack);

    enemy_factory->list_combat_unit[3]->NewTurn();
    enemy_factory->list_combat_unit[3]->Go(Direction::Up);
    EXPECT_EQ(map->combat(location_for_enemy.Direction(Direction::Up)), nullptr);

    auto enemy_unit = enemy_factory->list_combat_unit[0];
    EXPECT_EQ(enemy_unit->health, enemy_unit->MAX_HEALTH - my_wizard->damage);
    enemy_unit->NewTurn();
    enemy_unit->Go(Direction::Up);
    EXPECT_EQ(map->combat(location_for_enemy), enemy_unit);
    enemy_unit->NewTurn();
    enemy_unit->Go(Direction::Up);
    EXPECT_EQ(map->combat(location_for_enemy.Direction(Direction::Up)), enemy_unit);

    for(int index = 1; index < 9; ++index) {
        EXPECT_EQ(enemy_factory->list_combat_unit[index]->health, enemy_factory->list_combat_unit[index]->MAX_HEALTH);
    }

    delete my_factory;
    delete enemy_factory;
    delete map;
}

TEST(EarthWizard, Attack) {
    Money money;
    money.Add(11700, 11500 , 12100);
    auto map = new Map();
    auto enemy_factory = new UnitFactory(Player::Opponent, map, money, Race::Earth);
    Unit::opponent_unit_factory = enemy_factory;

    Location location_for_enemy = Location(4, 5);

    create_9_unit(enemy_factory, location_for_enemy);

    auto my_factory = new UnitFactory(Player::Me, map, money, Race::Earth);
    Unit::my_unit_factory = my_factory;

    Location location_for_wizard = location_for_enemy.Direction(Direction::Up).Direction(Direction::Up).Direction(Direction::Right);

    auto city = new City(location_for_wizard);
    city->BuildArcherTower(money);
    city->BuildWizardTower(money);
    my_factory->AddWizard(city);
    delete city;

    auto my_wizard = my_factory->list_combat_unit[0];

    my_wizard->NewTurn();
    vector<Direction> to_attack = {Direction::Down, Direction::Down, Direction::Left};
    my_wizard->Attack(to_attack);

    EXPECT_EQ(enemy_factory->list_combat_unit[0]->health, enemy_factory->list_combat_unit[0]->MAX_HEALTH - my_wizard->damage);

    for(int index = 1; index < 5; ++index) {
        EXPECT_EQ(enemy_factory->list_combat_unit[index]->health, enemy_factory->list_combat_unit[index]->MAX_HEALTH - my_wizard->damage / 2);
    }

    for(int index = 5; index < 9; ++index) {
        EXPECT_EQ(enemy_factory->list_combat_unit[index]->health, enemy_factory->list_combat_unit[index]->MAX_HEALTH);
    }

    delete my_factory;
    delete enemy_factory;
    delete map;
}

TEST(AirWizard, Attack) {
    Money money;
    money.Add(11700, 11500 , 12100);
    auto map = new Map();

    Location location_for_enemy = Location(4, 5);

    auto my_factory = new UnitFactory(Player::Me, map, money, Race::Air);
    Unit::my_unit_factory = my_factory;

    Location location_for_wizard = location_for_enemy.Direction(Direction::Up).Direction(Direction::Up).Direction(Direction::Right);

    auto city = new City(location_for_wizard);
    city->BuildArcherTower(money);
    city->BuildWizardTower(money);
    my_factory->AddWizard(city);
    delete city;

    create_9_unit(my_factory, location_for_enemy);

    auto my_wizard = my_factory->list_combat_unit[0];

    my_factory->list_combat_unit[1]->health -= my_wizard->damage + 5;

    my_wizard->NewTurn();
    vector<Direction> to_attack = {Direction::Down, Direction::Down, Direction::Left};
    my_wizard->Attack(to_attack);

    EXPECT_EQ(my_factory->list_combat_unit[1]->health, my_factory->list_combat_unit[1]->MAX_HEALTH - 5);

    my_wizard->NewTurn();
    my_wizard->Attack(to_attack);

    for(int index = 1; index < 10; ++index) {
        EXPECT_EQ(my_factory->list_combat_unit[index]->health, my_factory->list_combat_unit[index]->MAX_HEALTH);
    }

    delete my_factory;
    delete map;
}

TEST(FireWizard, Attack) {
    Money money;
    money.Add(11700, 11500 , 12100);
    auto map = new Map();
    auto enemy_factory = new UnitFactory(Player::Opponent, map, money, Race::Earth);
    Unit::opponent_unit_factory = enemy_factory;

    Location location_for_enemy = Location(4, 5);

    create_9_unit(enemy_factory, location_for_enemy);

    auto my_factory = new UnitFactory(Player::Me, map, money, Race::Fire);
    Unit::my_unit_factory = my_factory;

    Location location_for_wizard = location_for_enemy.Direction(Direction::Up).Direction(Direction::Up).Direction(Direction::Right);

    auto city = new City(location_for_wizard);
    city->BuildArcherTower(money);
    city->BuildWizardTower(money);
    my_factory->AddWizard(city);
    delete city;

    auto my_wizard = my_factory->list_combat_unit[0];

    my_wizard->NewTurn();
    vector<Direction> to_attack = {Direction::Down, Direction::Down, Direction::Left};
    my_wizard->Attack(to_attack);

    EXPECT_EQ(enemy_factory->list_combat_unit[0]->health, enemy_factory->list_combat_unit[0]->MAX_HEALTH);

    for(int index = 1; index < 9; ++index) {
        EXPECT_EQ(enemy_factory->list_combat_unit[index]->health, enemy_factory->list_combat_unit[index]->MAX_HEALTH - my_wizard->damage);
    }

    delete my_factory;
    delete enemy_factory;
    delete map;
}

TEST(WaterFactory, Add) {

    Money money;

    auto map = new Map();

    auto factory = new UnitFactory(Player::Me, map, money, Race::Water);

    auto new_city = new City(Location(5,6));

    Unit::my_unit_factory = factory;

    //test Colonist

    factory->AddColonist(new_city);

    EXPECT_EQ(factory->list_colonist.size(), 1);

    EXPECT_EQ(factory->list_colonist[0]->Info(), "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0");

    const int MAX_HEIGHT = 20;
    const int MAX_WIDTH = 20;

    string sample;
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    EXPECT_EQ(map->Info(), sample);

    //test Worker

    factory->AddWorker(new_city);

    EXPECT_EQ(factory->list_worker.size(), 0);

    delete new_city;

    new_city = new City(Location(7,8));

    factory->AddWorker(new_city);

    EXPECT_EQ(factory->list_worker.size(), 1);

    EXPECT_EQ(factory->list_worker[0]->Info(), "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    //cout << sample << endl;

    EXPECT_EQ(map->Info(), sample);

    //test Warrior

    factory->AddWarrior(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    EXPECT_EQ(factory->list_combat_unit[0]->Info(), "Warrior health: 100 of MAX_HEALTH: 100 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 25");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Warrior health: 100 of MAX_HEALTH: 100 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 25\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    EXPECT_EQ(map->Info(), sample);

    //test Archer

    delete new_city;

    new_city = new City(Location(3, 8));

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    new_city->BuildWizardTower(money);

    EXPECT_EQ(new_city->IsWizardTowerExist(), 0);

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    new_city->BuildArcherTower(money);

    EXPECT_EQ(new_city->IsArcherTowerExist(), 1);

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 2);

    EXPECT_EQ(factory->list_combat_unit[1]->Info(), "Archer health: 75 of MAX_HEALTH: 75 location:  x:3 y:8 already_move: 1 frozen: 0 damage: 50");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Warrior health: 100 of MAX_HEALTH: 100 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 25\n";
            } else if(i == 3 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Archer health: 75 of MAX_HEALTH: 75 location:  x:3 y:8 already_move: 1 frozen: 0 damage: 50\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    EXPECT_EQ(map->Info(), sample);

    //test Wizard

    delete new_city;

    new_city = new City(Location(9, 8));

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 2);

    new_city->BuildArcherTower(money);
    new_city->BuildWizardTower(money);

    EXPECT_EQ(new_city->IsWizardTowerExist(), 1);

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 3);

    EXPECT_EQ(factory->list_combat_unit[2]->Info(), "Wizard health: 150 of MAX_HEALTH: 150 location:  x:9 y:8 already_move: 1 frozen: 0 damage: 50");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Warrior health: 100 of MAX_HEALTH: 100 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 25\n";
            } else if(i == 3 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Archer health: 75 of MAX_HEALTH: 75 location:  x:3 y:8 already_move: 1 frozen: 0 damage: 50\n";
            } else if(i == 9 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Wizard health: 150 of MAX_HEALTH: 150 location:  x:9 y:8 already_move: 1 frozen: 0 damage: 50\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    EXPECT_EQ(map->Info(), sample);

    delete factory;
    delete map;
    delete new_city;
}

TEST(EarthFactory, Add) {

    Money money;

    auto map = new Map();

    auto factory = new UnitFactory(Player::Me, map, money, Race::Earth);

    auto new_city = new City(Location(5,6));

    Unit::my_unit_factory = factory;

    //test Colonist

    factory->AddColonist(new_city);

    EXPECT_EQ(factory->list_colonist.size(), 1);

    EXPECT_EQ(factory->list_colonist[0]->Info(), "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0");

    const int MAX_HEIGHT = 20;
    const int MAX_WIDTH = 20;

    string sample;
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    EXPECT_EQ(map->Info(), sample);

    //test Worker

    factory->AddWorker(new_city);

    EXPECT_EQ(factory->list_worker.size(), 0);

    delete new_city;

    new_city = new City(Location(7,8));

    factory->AddWorker(new_city);

    EXPECT_EQ(factory->list_worker.size(), 1);

    EXPECT_EQ(factory->list_worker[0]->Info(), "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    //cout << sample << endl;

    EXPECT_EQ(map->Info(), sample);

    //test Warrior

    factory->AddWarrior(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    EXPECT_EQ(factory->list_combat_unit[0]->Info(), "Warrior health: 120 of MAX_HEALTH: 120 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 20");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Warrior health: 120 of MAX_HEALTH: 120 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 20\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    EXPECT_EQ(map->Info(), sample);

    //test Archer


    delete new_city;

    new_city = new City(Location(3, 8));

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    new_city->BuildWizardTower(money);

    EXPECT_EQ(new_city->IsWizardTowerExist(), 0);

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    new_city->BuildArcherTower(money);

    EXPECT_EQ(new_city->IsArcherTowerExist(), 1);

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 2);

    EXPECT_EQ(factory->list_combat_unit[1]->Info(), "Archer health: 90 of MAX_HEALTH: 90 location:  x:3 y:8 already_move: 1 frozen: 0 damage: 40");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Warrior health: 120 of MAX_HEALTH: 120 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 20\n";
            } else if(i == 3 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Archer health: 90 of MAX_HEALTH: 90 location:  x:3 y:8 already_move: 1 frozen: 0 damage: 40\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    EXPECT_EQ(map->Info(), sample);

    //test Wizard

    delete new_city;

    new_city = new City(Location(9, 8));

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 2);

    new_city->BuildArcherTower(money);
    new_city->BuildWizardTower(money);

    EXPECT_EQ(new_city->IsWizardTowerExist(), 1);

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 3);

    EXPECT_EQ(factory->list_combat_unit[2]->Info(), "Wizard health: 170 of MAX_HEALTH: 170 location:  x:9 y:8 already_move: 1 frozen: 0 damage: 30");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Warrior health: 120 of MAX_HEALTH: 120 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 20\n";
            } else if(i == 3 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Archer health: 90 of MAX_HEALTH: 90 location:  x:3 y:8 already_move: 1 frozen: 0 damage: 40\n";
            } else if(i == 9 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Wizard health: 170 of MAX_HEALTH: 170 location:  x:9 y:8 already_move: 1 frozen: 0 damage: 30\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    EXPECT_EQ(map->Info(), sample);

    delete factory;
    delete map;
    delete new_city;
}

TEST(AirFactory, Add) {

    Money money;

    auto map = new Map();

    auto factory = new UnitFactory(Player::Me, map, money, Race::Air);

    auto new_city = new City(Location(5,6));

    Unit::my_unit_factory = factory;

    //test Colonist

    factory->AddColonist(new_city);

    EXPECT_EQ(factory->list_colonist.size(), 1);

    EXPECT_EQ(factory->list_colonist[0]->Info(), "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0");

    const int MAX_HEIGHT = 20;
    const int MAX_WIDTH = 20;

    string sample;
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    EXPECT_EQ(map->Info(), sample);

    //test Worker

    factory->AddWorker(new_city);

    EXPECT_EQ(factory->list_worker.size(), 0);

    delete new_city;

    new_city = new City(Location(7,8));

    factory->AddWorker(new_city);

    EXPECT_EQ(factory->list_worker.size(), 1);

    EXPECT_EQ(factory->list_worker[0]->Info(), "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    //cout << sample << endl;

    EXPECT_EQ(map->Info(), sample);

    //test Warrior

    factory->AddWarrior(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    EXPECT_EQ(factory->list_combat_unit[0]->Info(), "Warrior health: 80 of MAX_HEALTH: 80 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 35");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Warrior health: 80 of MAX_HEALTH: 80 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 35\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    EXPECT_EQ(map->Info(), sample);

    //test Archer


    delete new_city;

    new_city = new City(Location(3, 8));

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    new_city->BuildWizardTower(money);

    EXPECT_EQ(new_city->IsWizardTowerExist(), 0);

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    new_city->BuildArcherTower(money);

    EXPECT_EQ(new_city->IsArcherTowerExist(), 1);

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 2);

    EXPECT_EQ(factory->list_combat_unit[1]->Info(), "Archer health: 60 of MAX_HEALTH: 60 location:  x:3 y:8 already_move: 1 frozen: 0 damage: 55");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Warrior health: 80 of MAX_HEALTH: 80 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 35\n";
            } else if(i == 3 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Archer health: 60 of MAX_HEALTH: 60 location:  x:3 y:8 already_move: 1 frozen: 0 damage: 55\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    EXPECT_EQ(map->Info(), sample);

    //test Wizard

    delete new_city;

    new_city = new City(Location(9, 8));

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 2);

    new_city->BuildArcherTower(money);
    new_city->BuildWizardTower(money);

    EXPECT_EQ(new_city->IsWizardTowerExist(), 1);

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 3);

    EXPECT_EQ(factory->list_combat_unit[2]->Info(), "Wizard health: 140 of MAX_HEALTH: 140 location:  x:9 y:8 already_move: 1 frozen: 0 damage: 70");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Warrior health: 80 of MAX_HEALTH: 80 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 35\n";
            } else if(i == 3 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Archer health: 60 of MAX_HEALTH: 60 location:  x:3 y:8 already_move: 1 frozen: 0 damage: 55\n";
            } else if(i == 9 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Wizard health: 140 of MAX_HEALTH: 140 location:  x:9 y:8 already_move: 1 frozen: 0 damage: 70\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    EXPECT_EQ(map->Info(), sample);

    delete factory;
    delete map;
    delete new_city;
}

TEST(FireFactory, Add) {

    Money money;

    auto map = new Map();

    auto factory = new UnitFactory(Player::Me, map, money, Race::Fire);

    auto new_city = new City(Location(5,6));

    Unit::my_unit_factory = factory;

    //test Colonist

    factory->AddColonist(new_city);

    EXPECT_EQ(factory->list_colonist.size(), 1);

    EXPECT_EQ(factory->list_colonist[0]->Info(), "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0");

    const int MAX_HEIGHT = 20;
    const int MAX_WIDTH = 20;

    string sample;
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    EXPECT_EQ(map->Info(), sample);

    //test Worker

    factory->AddWorker(new_city);

    EXPECT_EQ(factory->list_worker.size(), 0);

    delete new_city;

    new_city = new City(Location(7,8));

    factory->AddWorker(new_city);

    EXPECT_EQ(factory->list_worker.size(), 1);

    EXPECT_EQ(factory->list_worker[0]->Info(), "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    //cout << sample << endl;

    EXPECT_EQ(map->Info(), sample);

    //test Warrior

    factory->AddWarrior(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    EXPECT_EQ(factory->list_combat_unit[0]->Info(), "Warrior health: 90 of MAX_HEALTH: 90 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 30");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Warrior health: 90 of MAX_HEALTH: 90 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 30\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    EXPECT_EQ(map->Info(), sample);

    //test Archer


    delete new_city;

    new_city = new City(Location(3, 8));

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    new_city->BuildWizardTower(money);

    EXPECT_EQ(new_city->IsWizardTowerExist(), 0);

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    new_city->BuildArcherTower(money);

    EXPECT_EQ(new_city->IsArcherTowerExist(), 1);

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 2);

    EXPECT_EQ(factory->list_combat_unit[1]->Info(), "Archer health: 70 of MAX_HEALTH: 70 location:  x:3 y:8 already_move: 1 frozen: 0 damage: 50");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Warrior health: 90 of MAX_HEALTH: 90 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 30\n";
            } else if(i == 3 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Archer health: 70 of MAX_HEALTH: 70 location:  x:3 y:8 already_move: 1 frozen: 0 damage: 50\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    EXPECT_EQ(map->Info(), sample);

    //test Wizard

    delete new_city;

    new_city = new City(Location(9, 8));

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 2);

    new_city->BuildArcherTower(money);
    new_city->BuildWizardTower(money);

    EXPECT_EQ(new_city->IsWizardTowerExist(), 1);

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 3);

    EXPECT_EQ(factory->list_combat_unit[2]->Info(), "Wizard health: 160 of MAX_HEALTH: 160 location:  x:9 y:8 already_move: 1 frozen: 0 damage: 15");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Warrior health: 90 of MAX_HEALTH: 90 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 30\n";
            } else if(i == 3 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Archer health: 70 of MAX_HEALTH: 70 location:  x:3 y:8 already_move: 1 frozen: 0 damage: 50\n";
            } else if(i == 9 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Wizard health: 160 of MAX_HEALTH: 160 location:  x:9 y:8 already_move: 1 frozen: 0 damage: 15\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Colonist\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if (i == 5 && j == 6) {
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 1 of MAX_HEALTH: 1 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 1 of MAX_HEALTH: 1 location:  x:7 y:8 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Cities\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
        }
    }

    EXPECT_EQ(map->Info(), sample);

    delete factory;
    delete map;
    delete new_city;
}

TEST(Map, Generate) {
    auto map = new Map();
    map->Generate();

    for(int i = 0; i < Map::MAX_HEIGHT; ++i) {
        for(int j = 0; j < Map::MAX_WIDTH; ++j) {
            cout << map->resource(Location(i, j)) << ' ';
            if(j < Map::MAX_WIDTH / 2) {
                EXPECT_EQ(map->resource(Location(i, j)), map->resource(Location(i, Map::MAX_WIDTH - 1 - j)));
            }
        }
        cout << '\n';
    }

    delete map;
}

TEST(Command, BuildCity) {
    Money money;
    money.Add(11700, 11500 , 12100);
    auto map = new Map();
    auto factory = new UnitFactory(Player::Me, map, money, Race::Fire);
    Unit::my_unit_factory = factory;
    auto enemy_factory = new UnitFactory(Player::Opponent, map, money, Race::Air);
    Unit::opponent_unit_factory = enemy_factory;
    auto my_city_factory = new CityFactory(Player::Me, map);
    City::my_city_factory = my_city_factory;
    auto opponent_city_factory = new CityFactory(Player::Opponent, map);
    City::opponent_city_factory = opponent_city_factory;
    ICommand::my_unit_factory = factory;
    ICommand::opponent_unit_factory = enemy_factory;
    ICommand::my_city_factory = my_city_factory;
    ICommand::opponent_city_factory = opponent_city_factory;

    ICommand* build_city = new BuildCity(0);

    auto temp_city = new City(Location(4, 5));
    factory->AddColonist(temp_city);
    delete temp_city;

    build_city->Do();

    EXPECT_NE(map->city(Location(4, 5)), nullptr);
    EXPECT_EQ(map->city(Location(4, 5))->which, Player::Me);

    ICommand::NewTurn();

    temp_city = new City(Location(5, 5));
    enemy_factory->AddColonist(temp_city);
    delete temp_city;

    build_city->Do();

    EXPECT_NE(map->city(Location(5, 5)), nullptr);
    EXPECT_EQ(map->city(Location(5, 5))->which, Player::Opponent);

    ICommand::NewTurn();

    delete build_city;
    delete factory;
    delete enemy_factory;
    delete my_city_factory;
    delete opponent_city_factory;
    delete map;
}

TEST(Command, CreateUnit) {
    Money money;
    money.Add(11700, 11500 , 12100);
    auto map = new Map();
    auto factory = new UnitFactory(Player::Me, map, money, Race::Fire);
    Unit::my_unit_factory = factory;
    auto enemy_factory = new UnitFactory(Player::Opponent, map, money, Race::Air);
    Unit::opponent_unit_factory = enemy_factory;
    auto my_city_factory = new CityFactory(Player::Me, map);
    City::my_city_factory = my_city_factory;
    auto opponent_city_factory = new CityFactory(Player::Opponent, map);
    City::opponent_city_factory = opponent_city_factory;
    ICommand::my_unit_factory = factory;
    ICommand::opponent_unit_factory = enemy_factory;
    ICommand::my_city_factory = my_city_factory;
    ICommand::opponent_city_factory = opponent_city_factory;

    ICommand* build_city = new BuildCity(0);

    Location my_city_location(4, 5);

    auto temp_city = new City(my_city_location);
    factory->AddColonist(temp_city);
    delete temp_city;

    build_city->Do();

    EXPECT_NE(map->city(my_city_location), nullptr);
    EXPECT_EQ(map->city(my_city_location)->which, Player::Me);

    ICommand* create_worker = new CreateUnit(UnitType::WorkerType, 0);
    create_worker->Do();

    EXPECT_EQ(map->worker(my_city_location), factory->list_worker[0]);

    ICommand::NewTurn();

    Location opponent_city_location(5, 5);

    temp_city = new City(opponent_city_location);
    enemy_factory->AddColonist(temp_city);
    delete temp_city;

    build_city->Do();

    EXPECT_NE(map->city(opponent_city_location), nullptr);
    EXPECT_EQ(map->city(opponent_city_location)->which, Player::Opponent);

    ICommand* create_archer = new CreateUnit(UnitType::ArcherType, 0);
    create_archer->Do();

    EXPECT_EQ(map->combat(opponent_city_location), nullptr);

    map->city(opponent_city_location)->BuildArcherTower(money);

    create_archer->Do();

    EXPECT_EQ(map->combat(opponent_city_location), enemy_factory->list_combat_unit[0]);

    ICommand::NewTurn();

    delete create_worker;
    delete create_archer;
    delete build_city;
    delete factory;
    delete enemy_factory;
    delete my_city_factory;
    delete opponent_city_factory;
    delete map;
}

TEST(Command, CreateTower) {
    Money my_money;
    my_money.Add(11700, 11500 , 12100);
    Money opponent_money;
    opponent_money.Add(11700, 11500 , 12100);
    auto map = new Map();
    auto factory = new UnitFactory(Player::Me, map, my_money, Race::Fire);
    Unit::my_unit_factory = factory;
    auto enemy_factory = new UnitFactory(Player::Opponent, map, opponent_money, Race::Air);
    Unit::opponent_unit_factory = enemy_factory;
    auto my_city_factory = new CityFactory(Player::Me, map);
    City::my_city_factory = my_city_factory;
    auto opponent_city_factory = new CityFactory(Player::Opponent, map);
    City::opponent_city_factory = opponent_city_factory;
    ICommand::my_unit_factory = factory;
    ICommand::opponent_unit_factory = enemy_factory;
    ICommand::my_city_factory = my_city_factory;
    ICommand::opponent_city_factory = opponent_city_factory;

    ICommand* build_city = new BuildCity(0);
    ICommand* build_archer_tower = new CreateTower(TowerType::ArcherTower, 0);
    ICommand* build_wizard_tower = new CreateTower(TowerType::WizardTower, 0);
    
    Location my_city_location(4, 5);

    auto temp_city = new City(my_city_location);
    factory->AddColonist(temp_city);
    delete temp_city;

    build_city->Do();

    EXPECT_NE(map->city(my_city_location), nullptr);
    EXPECT_EQ(map->city(my_city_location)->which, Player::Me);

    ICommand* create_archer = new CreateUnit(UnitType::ArcherType, 0);
    ICommand* create_wizard = new CreateUnit(UnitType::WizardType, 0);

    create_archer->Do();
    create_wizard->Do();

    EXPECT_EQ(map->worker(my_city_location), nullptr);
    EXPECT_EQ(factory->list_combat_unit.size(), 0);

    build_archer_tower->Do();
    
    create_archer->Do();
    create_wizard->Do();

    EXPECT_EQ(factory->list_combat_unit.size(), 1);
    EXPECT_EQ(map->combat(my_city_location), factory->list_combat_unit[0]);

    factory->list_combat_unit[0]->NewTurn();
    factory->list_combat_unit[0]->Go(Direction::Down);

    build_wizard_tower->Do();

    create_wizard->Do();

    EXPECT_EQ(factory->list_combat_unit.size(), 2);
    EXPECT_EQ(map->combat(my_city_location), factory->list_combat_unit[1]);

    ICommand::NewTurn();

    Location opponent_city_location(5, 5);

    temp_city = new City(opponent_city_location);
    enemy_factory->AddColonist(temp_city);
    delete temp_city;

    build_city->Do();

    create_archer->Do();
    create_wizard->Do();

    EXPECT_EQ(map->combat(opponent_city_location), nullptr);
    EXPECT_EQ(enemy_factory->list_combat_unit.size(), 0);

    build_archer_tower->Do();

    create_archer->Do();
    create_wizard->Do();

    EXPECT_EQ(enemy_factory->list_combat_unit.size(), 1);
    EXPECT_EQ(map->combat(opponent_city_location), enemy_factory->list_combat_unit[0]);

    enemy_factory->list_combat_unit[0]->NewTurn();
    enemy_factory->list_combat_unit[0]->Go(Direction::Up);

    build_wizard_tower->Do();

    create_wizard->Do();

    EXPECT_EQ(enemy_factory->list_combat_unit.size(), 2);
    EXPECT_EQ(map->combat(opponent_city_location), enemy_factory->list_combat_unit[1]);

    ICommand::NewTurn();

    delete build_archer_tower;
    delete build_wizard_tower;
    delete create_wizard;
    delete create_archer;
    delete build_city;
    delete factory;
    delete enemy_factory;
    delete my_city_factory;
    delete opponent_city_factory;
    delete map;
}

TEST(Command, End) {
    Money my_money;
    my_money.Add(11700, 11500 , 12100);
    Money opponent_money;
    opponent_money.Add(11700, 11500 , 12100);
    auto map = new Map();
    auto factory = new UnitFactory(Player::Me, map, my_money, Race::Fire);
    Unit::my_unit_factory = factory;
    auto enemy_factory = new UnitFactory(Player::Opponent, map, opponent_money, Race::Air);
    Unit::opponent_unit_factory = enemy_factory;
    auto my_city_factory = new CityFactory(Player::Me, map);
    City::my_city_factory = my_city_factory;
    auto opponent_city_factory = new CityFactory(Player::Opponent, map);
    City::opponent_city_factory = opponent_city_factory;
    ICommand::my_unit_factory = factory;
    ICommand::opponent_unit_factory = enemy_factory;
    ICommand::my_city_factory = my_city_factory;
    ICommand::opponent_city_factory = opponent_city_factory;

    ICommand* build_city = new BuildCity(0);
    ICommand* build_archer_tower = new CreateTower(TowerType::ArcherTower, 0);
    ICommand* build_wizard_tower = new CreateTower(TowerType::WizardTower, 0);
    ICommand* end = new End();

    Location my_city_location(4, 5);

    auto temp_city = new City(my_city_location);
    factory->AddColonist(temp_city);
    delete temp_city;

    build_city->Do();

    EXPECT_NE(map->city(my_city_location), nullptr);
    EXPECT_EQ(map->city(my_city_location)->which, Player::Me);

    ICommand* create_archer = new CreateUnit(UnitType::ArcherType, 0);
    ICommand* create_wizard = new CreateUnit(UnitType::WizardType, 0);

    create_archer->Do();
    create_wizard->Do();

    EXPECT_EQ(map->worker(my_city_location), nullptr);
    EXPECT_EQ(factory->list_combat_unit.size(), 0);

    build_archer_tower->Do();

    create_archer->Do();
    create_wizard->Do();

    EXPECT_EQ(factory->list_combat_unit.size(), 1);
    EXPECT_EQ(map->combat(my_city_location), factory->list_combat_unit[0]);

    factory->list_combat_unit[0]->NewTurn();
    factory->list_combat_unit[0]->Go(Direction::Down);

    build_wizard_tower->Do();

    create_wizard->Do();

    EXPECT_EQ(factory->list_combat_unit.size(), 2);
    EXPECT_EQ(map->combat(my_city_location), factory->list_combat_unit[1]);

    Location opponent_city_location(5, 5);

    temp_city = new City(opponent_city_location);
    enemy_factory->AddColonist(temp_city);
    delete temp_city;
    opponent_city_factory->AddCity(enemy_factory->list_colonist[0]);
    opponent_city_factory->cities.push_back(nullptr);

    EXPECT_EQ(end->Do(), 0);
    EXPECT_EQ(opponent_city_factory->cities.size(), 1);

    ICommand* create_warrior = new CreateUnit(UnitType::WarriorType, 0);

    create_warrior->Do();

    factory->list_combat_unit[1]->NewTurn();
    factory->list_combat_unit[1]->Go(Direction::Left);
    enemy_factory->list_combat_unit[0]->NewTurn();
    enemy_factory->list_combat_unit[0]->Go(Direction::Down);

    EXPECT_EQ(end->Do(), 1);

    delete end;
    delete build_archer_tower;
    delete build_wizard_tower;
    delete create_wizard;
    delete create_warrior;
    delete create_archer;
    delete build_city;
    delete factory;
    delete enemy_factory;
    delete my_city_factory;
    delete opponent_city_factory;
    delete map;
}

TEST(Command, Mine) {
    Money my_money;
    my_money.Add(11700, 11500 , 12100);
    auto map = new Map();
    map->Generate();
    auto factory = new UnitFactory(Player::Me, map, my_money, Race::Fire);
    Unit::my_unit_factory = factory;
    auto my_city_factory = new CityFactory(Player::Me, map);
    City::my_city_factory = my_city_factory;
    ICommand::my_unit_factory = factory;
    ICommand::my_city_factory = my_city_factory;

    Location my_city_location(0, 0);

    auto temp_city = new City(my_city_location);
    factory->AddColonist(temp_city);
    delete temp_city;

    ICommand* build_city = new BuildCity(0);
    build_city->Do();

    ICommand* create_worker = new CreateUnit(UnitType::WorkerType, 0);
    create_worker->Do();

    Location worker_location = my_city_location;

    ICommand* mine = new Mine(0);

    while(worker_location.IsOnField()) {
        if(map->resource(my_city_location) != Resource::Nothing) {
            factory->list_worker[0]->NewTurn();
            mine->Do();
            EXPECT_EQ(factory->list_worker[0]->cary_gold + factory->list_worker[0]->cary_silver + factory->list_worker[0]->cary_wood, 100);
        }

        worker_location = worker_location.Direction(Direction::Up);
        factory->list_worker[0]->NewTurn();
        factory->list_worker[0]->Go(Direction::Up);
    }

    delete build_city;
    delete create_worker;
    delete mine;
    delete factory;
    delete my_city_factory;
    delete map;
}

TEST(Command, Go) {
    Money my_money;
    my_money.Add(11700, 11500 , 12100);
    auto map = new Map();
    map->Generate();
    auto factory = new UnitFactory(Player::Me, map, my_money, Race::Fire);
    Unit::my_unit_factory = factory;
    auto my_city_factory = new CityFactory(Player::Me, map);
    City::my_city_factory = my_city_factory;
    ICommand::my_unit_factory = factory;
    ICommand::my_city_factory = my_city_factory;

    Location my_city_location(0, 0);

    auto temp_city = new City(my_city_location);
    factory->AddColonist(temp_city);
    delete temp_city;

    temp_city = new City(my_city_location.Direction(Direction::Right).Direction(Direction::Right).Direction(Direction::Right));
    factory->AddColonist(temp_city);
    delete temp_city;

    ICommand* build_city = new BuildCity(0);
    EXPECT_EQ(build_city->Do(), 0);

    ICommand* build_city_1 = new BuildCity(1);
    EXPECT_EQ(build_city_1->Do(), 0);

    ICommand* create_worker_0 = new CreateUnit(UnitType::WorkerType, 1);
    EXPECT_EQ(create_worker_0->Do(), 0);

    ICommand* create_worker = new CreateUnit(UnitType::WorkerType, 0);
    EXPECT_EQ(create_worker->Do(), 0);

    ICommand* create_warrior = new CreateUnit(UnitType::WarriorType, 0);
    EXPECT_EQ(create_warrior->Do(), 0);

    ICommand* worker_go = new Go(UnitCommonType::WorkerCommonType, 1, Up);
    factory->list_worker[1]->NewTurn();
    EXPECT_EQ(worker_go->Do(), 0);
    EXPECT_EQ(map->worker(my_city_location.Direction(Direction::Up)), factory->list_worker[1]);
    factory->list_worker[1]->NewTurn();
    EXPECT_EQ(worker_go->Do(), 0);
    EXPECT_EQ(map->worker(my_city_location.Direction(Direction::Up).Direction(Direction::Up)), factory->list_worker[1]);
    EXPECT_EQ(worker_go->Do(), 1);

    ICommand* warrior_go = new Go(UnitCommonType::CombatCommonType, 0, Direction::Right);
    factory->list_combat_unit[0]->NewTurn();
    EXPECT_EQ(warrior_go->Do(), 0);
    EXPECT_EQ(map->combat(my_city_location.Direction(Direction::Right)), factory->list_combat_unit[0]);

    ICommand* create_colonist = new CreateUnit(UnitType::ColonistType, 0);
    EXPECT_EQ(create_colonist->Do(), 0);

    ICommand* colonist_go = new Go(UnitCommonType::ColonistCommonType, 0, Right);
    factory->list_colonist[0]->NewTurn();
    EXPECT_EQ(colonist_go->Do(), 0);
    EXPECT_EQ(map->colonist(my_city_location.Direction(Direction::Right)), factory->list_colonist[0]);

    delete worker_go;
    delete warrior_go;
    delete colonist_go;
    delete build_city;
    delete build_city_1;
    delete create_worker;
    delete create_worker_0;
    delete create_warrior;
    delete create_colonist;
    delete factory;
    delete my_city_factory;
    delete map;
}

TEST(Command, Attack) {
    Money my_money;
    my_money.Add(11700, 11500 , 12100);
    Money opponent_money;
    opponent_money.Add(11700, 11500 , 12100);
    auto map = new Map();
    auto factory = new UnitFactory(Player::Me, map, my_money, Race::Fire);
    Unit::my_unit_factory = factory;
    auto enemy_factory = new UnitFactory(Player::Opponent, map, opponent_money, Race::Air);
    Unit::opponent_unit_factory = enemy_factory;
    auto my_city_factory = new CityFactory(Player::Me, map);
    City::my_city_factory = my_city_factory;
    auto opponent_city_factory = new CityFactory(Player::Opponent, map);
    City::opponent_city_factory = opponent_city_factory;
    ICommand::my_unit_factory = factory;
    ICommand::opponent_unit_factory = enemy_factory;
    ICommand::my_city_factory = my_city_factory;
    ICommand::opponent_city_factory = opponent_city_factory;

    ICommand* build_city = new BuildCity(0);
    ICommand* build_archer_tower = new CreateTower(TowerType::ArcherTower, 0);
    ICommand* build_wizard_tower = new CreateTower(TowerType::WizardTower, 0);
    ICommand* end = new End();

    Location my_city_location(4, 5);

    auto temp_city = new City(my_city_location);
    factory->AddColonist(temp_city);
    delete temp_city;

    build_city->Do();

    EXPECT_NE(map->city(my_city_location), nullptr);
    EXPECT_EQ(map->city(my_city_location)->which, Player::Me);

    ICommand* create_archer = new CreateUnit(UnitType::ArcherType, 0);
    ICommand* create_wizard = new CreateUnit(UnitType::WizardType, 0);

    create_archer->Do();
    create_wizard->Do();

    EXPECT_EQ(map->combat(my_city_location), nullptr);
    EXPECT_EQ(factory->list_combat_unit.size(), 0);

    build_archer_tower->Do();

    create_archer->Do();
    create_wizard->Do();

    EXPECT_EQ(factory->list_combat_unit.size(), 1);
    EXPECT_EQ(map->combat(my_city_location), factory->list_combat_unit[0]);

    factory->list_combat_unit[0]->NewTurn();
    factory->list_combat_unit[0]->Go(Direction::Down);

    build_wizard_tower->Do();

    create_wizard->Do();

    EXPECT_EQ(factory->list_combat_unit.size(), 2);
    EXPECT_EQ(map->combat(my_city_location), factory->list_combat_unit[1]);

    Location opponent_city_location(5, 5);

    temp_city = new City(opponent_city_location);
    enemy_factory->AddColonist(temp_city);
    delete temp_city;
    opponent_city_factory->AddCity(enemy_factory->list_colonist[0]);
    opponent_city_factory->cities.push_back(nullptr);

    EXPECT_EQ(end->Do(), 0);
    EXPECT_EQ(opponent_city_factory->cities.size(), 1);

    build_archer_tower->Do();
    create_archer->Do();

    vector<Direction> vec_1(1, Direction::Right);
    vector<Direction> vec_2(2, Direction::Down);

    ICommand* attack_1_down = new Attack(0, vec_1);
    ICommand* attack_2_down = new Attack(0, vec_2);

    EXPECT_EQ(end->Do(), 0);
    EXPECT_EQ(end->Do(), 0);

    EXPECT_EQ(attack_1_down->Do(), 1);

    EXPECT_EQ(end->Do(), 0);
    EXPECT_EQ(end->Do(), 0);
    EXPECT_EQ(attack_2_down->Do(), 0);
    EXPECT_EQ(factory->list_combat_unit[0]->health, factory->list_combat_unit[0]->MAX_HEALTH - enemy_factory->list_combat_unit[0]->damage);

    EXPECT_EQ(end->Do(), 0);

    delete end;
    delete attack_1_down;
    delete attack_2_down;
    delete build_archer_tower;
    delete build_wizard_tower;
    delete create_wizard;
    delete create_archer;
    delete build_city;
    delete factory;
    delete enemy_factory;
    delete my_city_factory;
    delete opponent_city_factory;
    delete map;
}

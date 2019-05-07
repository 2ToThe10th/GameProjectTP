#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <CityFactory.h>
#include <Unit.h>
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
    money.Add(1700, 1500 , 2100);
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

TEST(WaterFactory, add) {

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

    new_city->location = Location(7,8);

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


    new_city->location = Location(3, 8);

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

    new_city->location = Location(9, 8);

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 2);

    new_city->BuildWizardTower(money);

    EXPECT_EQ(new_city->IsWizardTowerExist(), 1);

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 3);

    EXPECT_EQ(factory->list_combat_unit[2]->Info(), "Wizard health: 150 of MAX_HEALTH: 150 location:  x:9 y:8 already_move: 1 frozen: 0 damage: 90");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Warrior health: 100 of MAX_HEALTH: 100 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 25\n";
            } else if(i == 3 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Archer health: 75 of MAX_HEALTH: 75 location:  x:3 y:8 already_move: 1 frozen: 0 damage: 50\n";
            } else if(i == 9 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Wizard health: 150 of MAX_HEALTH: 150 location:  x:9 y:8 already_move: 1 frozen: 0 damage: 90\n";
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

TEST(EarthFactory, add) {

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

    new_city->location = Location(7,8);

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


    new_city->location = Location(3, 8);

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

    new_city->location = Location(9, 8);

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 2);

    new_city->BuildWizardTower(money);

    EXPECT_EQ(new_city->IsWizardTowerExist(), 1);

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 3);

    EXPECT_EQ(factory->list_combat_unit[2]->Info(), "Wizard health: 170 of MAX_HEALTH: 170 location:  x:9 y:8 already_move: 1 frozen: 0 damage: 70");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Warrior health: 120 of MAX_HEALTH: 120 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 20\n";
            } else if(i == 3 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Archer health: 90 of MAX_HEALTH: 90 location:  x:3 y:8 already_move: 1 frozen: 0 damage: 40\n";
            } else if(i == 9 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Wizard health: 170 of MAX_HEALTH: 170 location:  x:9 y:8 already_move: 1 frozen: 0 damage: 70\n";
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

TEST(AirFactory, add) {

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

    new_city->location = Location(7,8);

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


    new_city->location = Location(3, 8);

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

    new_city->location = Location(9, 8);

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 2);

    new_city->BuildWizardTower(money);

    EXPECT_EQ(new_city->IsWizardTowerExist(), 1);

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 3);

    EXPECT_EQ(factory->list_combat_unit[2]->Info(), "Wizard health: 130 of MAX_HEALTH: 130 location:  x:9 y:8 already_move: 1 frozen: 0 damage: 110");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Warrior health: 80 of MAX_HEALTH: 80 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 35\n";
            } else if(i == 3 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Archer health: 60 of MAX_HEALTH: 60 location:  x:3 y:8 already_move: 1 frozen: 0 damage: 55\n";
            } else if(i == 9 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Wizard health: 130 of MAX_HEALTH: 130 location:  x:9 y:8 already_move: 1 frozen: 0 damage: 110\n";
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

TEST(FireFactory, add) {

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

    new_city->location = Location(7,8);

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


    new_city->location = Location(3, 8);

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

    new_city->location = Location(9, 8);

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 2);

    new_city->BuildWizardTower(money);

    EXPECT_EQ(new_city->IsWizardTowerExist(), 1);

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 3);

    EXPECT_EQ(factory->list_combat_unit[2]->Info(), "Wizard health: 160 of MAX_HEALTH: 160 location:  x:9 y:8 already_move: 1 frozen: 0 damage: 80");

    sample = "";
    sample += "Combat\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Warrior health: 90 of MAX_HEALTH: 90 location:  x:7 y:8 already_move: 1 frozen: 0 damage: 30\n";
            } else if(i == 3 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Archer health: 70 of MAX_HEALTH: 70 location:  x:3 y:8 already_move: 1 frozen: 0 damage: 50\n";
            } else if(i == 9 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Wizard health: 160 of MAX_HEALTH: 160 location:  x:9 y:8 already_move: 1 frozen: 0 damage: 80\n";
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

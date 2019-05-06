#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <CityFactory.h>
#include "Location.h"
#include "Map.h"
#include "Money.h"
#include "UnitFactory.h"
#include "GameSocket.h"

using std::cin;
using std::cout;
using std::endl;
using std::to_string;

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

}

TEST(CityFactory, AddCity) {
    auto map = new Map();
    auto colonist = new Colonist(Player::Me, Race::Fire, 100, Location(0,0), map);
    auto city_factory = new CityFactory(Player::Me, map);

    EXPECT_EQ(city_factory->cities.size(), 0);

    city_factory->AddCity(colonist);
    EXPECT_EQ(city_factory->cities.size(), 1);

    city_factory->AddCity(colonist);
    EXPECT_EQ(city_factory->cities.size(), 1);

    city_factory->AddCity(nullptr);
    EXPECT_EQ(city_factory->cities.size(), 1);
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

TEST(WaterFactory, add) {

    Money money;

    auto map = new Map();

    auto factory = new UnitFactory(Player::Me, map, money, Race::Water);

    City new_city = City(Location(5,6));

    //test Colonist

    factory->AddColonist(new_city);

    EXPECT_EQ(factory->list_colonist.size(), 1);

    EXPECT_EQ(factory->list_colonist[0]->Info(), "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0");

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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
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

    new_city.location = Location(7,8);

    factory->AddWorker(new_city);

    EXPECT_EQ(factory->list_worker.size(), 1);

    EXPECT_EQ(factory->list_worker[0]->Info(), "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0");

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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0\n";
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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0\n";
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


    new_city.location = Location(3, 8);

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    new_city.BuildWizardTower(money);

    EXPECT_EQ(new_city.IsWizardTowerExist(), 0);

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    new_city.BuildArcherTower(money);

    EXPECT_EQ(new_city.IsArcherTowerExist(), 1);

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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0\n";
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

    new_city.location = Location(9, 8);

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 2);

    new_city.BuildWizardTower(money);

    EXPECT_EQ(new_city.IsWizardTowerExist(), 1);

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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0\n";
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
}

TEST(EarthFactory, add) {

    Money money;

    auto map = new Map();

    auto factory = new UnitFactory(Player::Me, map, money, Race::Earth);

    City new_city = City(Location(5,6));

    //test Colonist

    factory->AddColonist(new_city);

    EXPECT_EQ(factory->list_colonist.size(), 1);

    EXPECT_EQ(factory->list_colonist[0]->Info(), "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0");

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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
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

    new_city.location = Location(7,8);

    factory->AddWorker(new_city);

    EXPECT_EQ(factory->list_worker.size(), 1);

    EXPECT_EQ(factory->list_worker[0]->Info(), "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0");

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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0\n";
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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0\n";
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


    new_city.location = Location(3, 8);

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    new_city.BuildWizardTower(money);

    EXPECT_EQ(new_city.IsWizardTowerExist(), 0);

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    new_city.BuildArcherTower(money);

    EXPECT_EQ(new_city.IsArcherTowerExist(), 1);

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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0\n";
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

    new_city.location = Location(9, 8);

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 2);

    new_city.BuildWizardTower(money);

    EXPECT_EQ(new_city.IsWizardTowerExist(), 1);

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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0\n";
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
}

TEST(AirFactory, add) {

    Money money;

    auto map = new Map();

    auto factory = new UnitFactory(Player::Me, map, money, Race::Air);

    City new_city = City(Location(5,6));

    //test Colonist

    factory->AddColonist(new_city);

    EXPECT_EQ(factory->list_colonist.size(), 1);

    EXPECT_EQ(factory->list_colonist[0]->Info(), "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0");

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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
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

    new_city.location = Location(7,8);

    factory->AddWorker(new_city);

    EXPECT_EQ(factory->list_worker.size(), 1);

    EXPECT_EQ(factory->list_worker[0]->Info(), "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0");

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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0\n";
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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0\n";
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


    new_city.location = Location(3, 8);

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    new_city.BuildWizardTower(money);

    EXPECT_EQ(new_city.IsWizardTowerExist(), 0);

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    new_city.BuildArcherTower(money);

    EXPECT_EQ(new_city.IsArcherTowerExist(), 1);

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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0\n";
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

    new_city.location = Location(9, 8);

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 2);

    new_city.BuildWizardTower(money);

    EXPECT_EQ(new_city.IsWizardTowerExist(), 1);

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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0\n";
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
}

TEST(FireFactory, add) {

    Money money;

    auto map = new Map();

    auto factory = new UnitFactory(Player::Me, map, money, Race::Fire);

    City new_city = City(Location(5,6));

    //test Colonist

    factory->AddColonist(new_city);

    EXPECT_EQ(factory->list_colonist.size(), 1);

    EXPECT_EQ(factory->list_colonist[0]->Info(), "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0");

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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
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

    new_city.location = Location(7,8);

    factory->AddWorker(new_city);

    EXPECT_EQ(factory->list_worker.size(), 1);

    EXPECT_EQ(factory->list_worker[0]->Info(), "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0");

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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0\n";
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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0\n";
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


    new_city.location = Location(3, 8);

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    new_city.BuildWizardTower(money);

    EXPECT_EQ(new_city.IsWizardTowerExist(), 0);

    factory->AddArcher(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 1);

    new_city.BuildArcherTower(money);

    EXPECT_EQ(new_city.IsArcherTowerExist(), 1);

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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0\n";
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

    new_city.location = Location(9, 8);

    factory->AddWizard(new_city);

    EXPECT_EQ(factory->list_combat_unit.size(), 2);

    new_city.BuildWizardTower(money);

    EXPECT_EQ(new_city.IsWizardTowerExist(), 1);

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
                sample += to_string(i) + " " + to_string(j) + " " + "Colonist health: 50 of MAX_HEALTH: 50 location:  x:5 y:6 already_move: 1 frozen: 0\n";
            } else {
                sample += to_string(i) + " " + to_string(j) + " " + "nullptr\n";
            }
        }
    }

    sample += "Worker\n";

    for(int i = 0; i < MAX_HEIGHT; ++i) {
        for(int j =0; j < MAX_WIDTH; ++j) {
            if(i == 7 && j == 8) {
                sample += to_string(i) + " " + to_string(j) + " " + "Worker health: 25 of MAX_HEALTH: 25 location:  x:7 y:8 already_move: 1 frozen: 0\n";
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
}

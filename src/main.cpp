#include <iostream>
#include <string>
#include "GameSocket.h"
#include "UnitFactory.h"
#include "CityFactory.h"
#include "Map.h"

#ifdef DEBUG
#include <gtest/gtest.h>
#endif

using std::cin;
using std::cout;
using std::endl;

int Game();

int main(int argv, char* args[]) {
#ifdef DEBUG
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
#endif

    return Game();
}

int Game() {

    Player which_turn = Player::Me;

    GameSocket* socket = nullptr;

    cout << "Do you want to create a server or connect to exist?[S/C]" << endl;

    char user_answer;
    cin >> user_answer;

    if(user_answer == 'S' || user_answer == 's') {

        cout << "Choose port:" << endl;

        int port;
        cin >> port;

        socket = new GameSocket(port);

    }
    else if(user_answer == 'C' || user_answer == 'c') {

        cout << "Choose host:" << endl;

        char host[1000];
        if(scanf("%900s", host) != 1) {
            cout << "error: scanf host" << endl;
            throw -7;
        }

        cout << "Choose port:" << endl;

        int port;

        cin >> port;

        socket = new GameSocket(host, port);

        which_turn = Player::Opponent;

    }
    else {
        cout << "Wrong answer. Answer might be 'S' or 'C'" << endl;
        return 0;
    }

    auto map = new Map();

    Money money;

    cout << "Choose race(Water(w),Fire(f),Earth(e),Air(a)):" << endl;

    string string_to_send;

    string user_race;

    Race user_race_enum;

    while(string_to_send.empty()) {

        cin >> user_race;

        if (user_race == "Water" || user_race == "water" || user_race == "W" || user_race == "w") {
            user_race_enum = Race::Water;
            string_to_send = "w";
        }
        else
        if (user_race == "Air" || user_race == "air" || user_race == "A" || user_race == "a") {
            user_race_enum = Race::Air;
            string_to_send = "a";
        }
        else
        if (user_race == "Earth" || user_race == "earth" || user_race == "E" || user_race == "e") {
            user_race_enum = Race::Earth;
            string_to_send = "e";
        }
        else
        if (user_race == "Fire" || user_race == "fire" || user_race == "F" || user_race == "f") {
            user_race_enum = Race::Fire;
            string_to_send = "f";
        }
        else {
            cout << "It is not an exist race. Type one of the exist: Water, Fire, Earth, Air." << endl;
        }
    }

    auto my_factory = new UnitFactory(Player::Me, map, money, user_race_enum);

    cout << my_factory->Info() << endl;

    string opponent_race;

    if(which_turn == Player::Me) {
        socket->Write(string_to_send);
        opponent_race = socket->Read();
    }
    else {
        opponent_race = socket->Read();
        socket->Write(string_to_send);
    }

    Race opponent_race_enum;

    if (opponent_race == "w") {
        opponent_race_enum = Race::Water;
    }
    else if (opponent_race == "a") {
        opponent_race_enum = Race::Air;
    }
    else if (opponent_race == "e") {
        opponent_race_enum = Race::Earth;
    }
    else if (opponent_race == "f") {
        opponent_race_enum = Race::Fire;
    }
    else {
        cout << "Something wrong with socket" << endl;

        delete my_factory;
        delete socket;

        return 0;
    }

    auto opponent_factory = new UnitFactory(Player::Opponent, map, money, opponent_race_enum);

    cout << opponent_factory->Info() << endl;

    auto my_city_factory = new CityFactory(Player::Me, map);
    auto opponent_city_factory = new CityFactory(Player::Opponent, map);

    cout << my_city_factory->Info() << endl;
    cout << opponent_city_factory->Info() << endl;

    //TODO: generation of resources


//delete part

    delete socket;
    delete my_factory;
    delete opponent_factory;
    delete my_city_factory;
    delete opponent_city_factory;

    return 0;
}

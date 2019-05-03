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

int main(int argv, char* args[]) {
#ifdef DEBUG
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
#endif

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

    } else if(user_answer == 'C' || user_answer == 'c') {

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

    } else {
        cout << "Wrong answer. Answer might be 'S' or 'C'" << endl;
        return 0;
    }

    Map map;

    Money money;

    cout << "Choose race(Water(w),Fire(f),Earth(e),Air(a)):" << endl;

    string string_to_send;

    string user_race;

    UnitFactory* my_factory = nullptr;

    while(my_factory == nullptr) {

        cin >> user_race;

        if (user_race == "Water" || user_race == "water" || user_race == "W" || user_race == "w") {
            my_factory = new UnitFactory(Player::Me, map, money, Race::Water);
            string_to_send = "w";
        } else
        if (user_race == "Air" || user_race == "air" || user_race == "A" || user_race == "a") {
            my_factory = new UnitFactory(Player::Me, map, money, Race::Air);
            string_to_send = "a";
        } else
        if (user_race == "Earth" || user_race == "earth" || user_race == "E" || user_race == "e") {
            my_factory = new UnitFactory(Player::Me, map, money, Race::Earth);
            string_to_send = "e";
        } else
        if (user_race == "Fire" || user_race == "fire" || user_race == "F" || user_race == "f") {
            my_factory = new UnitFactory(Player::Me, map, money, Race::Fire);
            string_to_send = "f";
        } else {
            cout << "It is not an exist race. Type one of the exist: Water, Fire, Earth, Air." << endl;
        }

    }

    cout << my_factory->Info() << endl;

    string opponent_race;

    if(which_turn == Player::Me) {
        socket->Write(string_to_send);
        opponent_race = socket->Read();
    } else {
        opponent_race = socket->Read();
        socket->Write(string_to_send);
    }

    UnitFactory* opponent_factory = nullptr;

    if (opponent_race == "w") {
        opponent_factory = new UnitFactory(Player::Opponent, map, money, Race::Water);
        string_to_send = "w";
    } else
    if (opponent_race == "a") {
        opponent_factory = new UnitFactory(Player::Opponent, map, money, Race::Air);
        string_to_send = "a";
    } else
    if (opponent_race == "e") {
        opponent_factory = new UnitFactory(Player::Opponent, map, money, Race::Earth);
        string_to_send = "e";
    } else
    if (opponent_race == "f") {
        opponent_factory = new UnitFactory(Player::Opponent, map, money, Race::Fire);
        string_to_send = "f";
    } else {
        cout << "Something wrong with socket" << endl;

        delete my_factory;
        delete socket;

        return 0;
    }
    
    cout << opponent_factory->Info() << endl;

    CityFactory* my_city_factory = new CityFactory(Player::Me);
    CityFactory* opponent_city_factory = new CityFactory(Player::Opponent);

    cout << my_city_factory->Info() << endl;
    cout << opponent_city_factory->Info() << endl;

    //TODO: generation 0f resources


    

//delete part

    delete socket;
    delete my_factory;
    delete opponent_factory;
    delete my_city_factory;
    delete opponent_city_factory;

    return 0;
}

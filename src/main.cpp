#include <iostream>
#include <string>
#include "GameSocket.h"
#include "UnitFactory.h"
#include "WaterUnitFactory.h"
#include "AirUnitFactory.h"
#include "EarthUnitFactory.h"
#include "FireUnitFactory.h"
#include "CityFactory.h"

#ifdef DEBUG
#include "gtest/gtest.h"
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

        char* host;
        if(scanf("%ms", &host) != 1) {
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

    cout << "Choose race(Water(w),Fire(f),Earth(e),Air(a)):" << endl;

    string string_to_send;

    string user_race;

    UnitFactory* my_factory = nullptr;

    while(my_factory == nullptr) {

        cin >> user_race;

        if (user_race == "Water" || user_race == "water" || user_race == "W" || user_race == "w") {
            my_factory = new WaterUnitFactory(Player::Me);
            string_to_send = "w";
        } else
        if (user_race == "Air" || user_race == "air" || user_race == "A" || user_race == "a") {
            my_factory = new AirUnitFactory(Player::Me);
            string_to_send = "a";
        } else
        if (user_race == "Earth" || user_race == "earth" || user_race == "E" || user_race == "e") {
            my_factory = new EarthUnitFactory(Player::Me);
            string_to_send = "e";
        } else
        if (user_race == "Fire" || user_race == "fire" || user_race == "F" || user_race == "f") {
            my_factory = new FireUnitFactory(Player::Me);
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
        opponent_factory = new WaterUnitFactory(Player::Opponent);
        string_to_send = "w";
    } else
    if (opponent_race == "a") {
        opponent_factory = new AirUnitFactory(Player::Opponent);
        string_to_send = "a";
    } else
    if (opponent_race == "e") {
        opponent_factory = new EarthUnitFactory(Player::Opponent);
        string_to_send = "e";
    } else
    if (opponent_race == "f") {
        opponent_factory = new FireUnitFactory(Player::Opponent);
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

//delete part

    delete socket;
    delete my_factory;
    delete opponent_factory;
    delete my_city_factory;
    delete opponent_city_factory;

    return 0;
}

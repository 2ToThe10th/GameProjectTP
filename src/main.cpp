#include <iostream>
#include <string>
#include <Command/End.h>
#include "GameSocket.h"
#include "UnitFactory.h"
#include "CityFactory.h"
#include "Map.h"
#include "Command/ICommand.h"
#include "Interpreter.h"
#include "Graphic.h"
#include "CryptoSocket.h"

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

    Player which_first_turn = Player::Me;

    GameSocket* socket = nullptr;

    bool s_c_trouble = false;

    do {
        cout << "Do you want to create a server or connect to exist?[S/C]" << endl;
        char user_answer;
        cin >> user_answer;
        s_c_trouble = false;
        if (user_answer == 'S' || user_answer == 's') {
            bool trouble = false;
            do {
                trouble = false;
                try {
                    cout << "Choose port:" << endl;

                    string port_string;
                    cin >> port_string;

                    if(port_string.size() > 6) {
                        cout << "Port might be between 1 and 65535" << endl;
                        throw 9;
                    }

                    int port = 0;
                    for(auto i: port_string) {
                        if(i >= '0' && i <= '9') {
                            port = 10 * port + int(i - '0');
                        }
                    }

                    socket = new CryptoSocket(port);
                }
                catch (...) {

                    cout << "Please retype your input. Check pronunciation" << endl;
                    trouble = true;
                }

            } while(trouble);

        } else if (user_answer == 'C' || user_answer == 'c') {
            bool trouble = false;
            do {
                trouble = false;
                try {

                    cout << "Choose host:" << endl;

                    char host[1000];
                    if (scanf("%900s", host) != 1) {
                        cout << "error: scanf host" << endl;
                        throw -7;
                    }

                    cout << "Choose port:" << endl;

                    string port_string;
                    cin >> port_string;

                    if(port_string.size() > 6) {
                        cout << "Port might be between 1 and 65535" << endl;
                        throw 9;
                    }

                    int port = 0;
                    for(auto i: port_string) {
                        if(i >= '0' && i <= '9') {
                            port = 10 * port + int(i - '0');
                        }
                    }

                    socket = new CryptoSocket(host, port);

                    which_first_turn = Player::Opponent;
                }
                catch (...) {
                    cout << "Please retype your input. Check pronunciation" << endl;
                    trouble = true;
                }
            } while(trouble);

        } else {
            cout << "Wrong answer. Answer might be 'S' or 'C'" << endl;
            s_c_trouble = true;
        }
    } while(s_c_trouble);

    auto map = new Map();

    Money my_money;
    Money opponent_money;

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

    auto my_factory = new UnitFactory(Player::Me, map, my_money, user_race_enum);

    cout << my_factory->Info() << endl;

    string opponent_race;

    if(which_first_turn == Player::Me) {
        socket->Write(string_to_send);
        opponent_race = socket->Read();
    }
    else {
        opponent_race = socket->Read();
        socket->Write(string_to_send);
    }

    cout << opponent_race << endl;

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

    auto opponent_factory = new UnitFactory(Player::Opponent, map, opponent_money, opponent_race_enum);

    cout << opponent_factory->Info() << endl;

    Unit::my_unit_factory = my_factory;
    Unit::opponent_unit_factory = opponent_factory;

    auto my_city_factory = new CityFactory(Player::Me, map);
    auto opponent_city_factory = new CityFactory(Player::Opponent, map);

    cout << my_city_factory->Info() << endl;
    cout << opponent_city_factory->Info() << endl;

    Unit::my_unit_factory = my_factory;
    Unit::opponent_unit_factory = opponent_factory;
    City::my_city_factory = my_city_factory;
    City::opponent_city_factory = opponent_city_factory;
    ICommand::socket = socket;
    ICommand::my_unit_factory = my_factory;
    ICommand::opponent_unit_factory = opponent_factory;
    ICommand::my_city_factory = my_city_factory;
    ICommand::opponent_city_factory = opponent_city_factory;

    unsigned int seed;

    if(which_first_turn == Player::Me) {
        seed = time(nullptr);
        string seed_string = std::to_string(seed);
        socket->Write(seed_string);
    }
    else {
        seed = std::stoi(socket->Read());
    }

    map->Generate(seed);

    Location my_first_city_location(0, 0);
    Location opponent_first_city_location(0, 0);

    if(which_first_turn == Player::Me) {
        my_first_city_location = Location(9, 6);
        opponent_first_city_location = Location(9, 13);
    }
    else{
        my_first_city_location = Location(9, 13);
        opponent_first_city_location = Location(9, 6);
    }

    auto temporary_city = new City(my_first_city_location);
    my_factory->AddColonist(temporary_city);
    delete temporary_city;

    my_city_factory->AddCity(my_factory->list_colonist[0]);
    my_factory->AddWarrior(my_city_factory->cities[0]);
    my_factory->list_combat_unit[0]->NewTurn();
    my_factory->AddWorker(my_city_factory->cities[0]);
    my_factory->list_worker[0]->NewTurn();

    temporary_city = new City(opponent_first_city_location);
    opponent_factory->AddColonist(temporary_city);
    delete temporary_city;

    opponent_city_factory->AddCity(opponent_factory->list_colonist[0]);
    opponent_factory->AddWarrior(opponent_city_factory->cities[0]);
    opponent_factory->list_combat_unit[0]->NewTurn();
    opponent_factory->AddWorker(opponent_city_factory->cities[0]);
    opponent_factory->list_worker[0]->NewTurn();

    if(which_first_turn == Player::Opponent) {
        ICommand* end = new End();
        end->Do();
        ICommand::is_end = false;
    }

    Graphic graphic(map);

    graphic.Draw();
    if(ICommand::which_turn == Player::Me) {
        cout << "Your turn\n";
    }
    cout << "Your money:" << my_money.Info() << '\n' << "Your opponent money:" << opponent_money.Info() << endl;

    while(true) {
        string string_command;
        if(ICommand::which_turn == Player::Me) {
            getline(cin, string_command);
        }
        else {
            string_command = socket->Read();
            cout << string_command << endl;
        }
        Interpreter interpreter(string_command);
        ICommand* command = interpreter.Translate();

        unsigned int command_result = command->Do();

        if(command_result == 0) {
            if((ICommand::which_turn == Player::Me && !ICommand::is_end) ||
                    (ICommand::which_turn == Player::Opponent && ICommand::is_end)) {
                command->Send();
            }
            ICommand::is_end = false;
        }
        else if(ICommand::is_end) {
            if(ICommand::which_turn == Player::Opponent) {
                command->Send();
            }
            ICommand::is_end = false;
            break;
        }
        else {
            continue;
        }
        graphic.Draw();
        if(ICommand::which_turn == Player::Me) {
            cout << "Your turn\n";
        }
        cout << "Your money:" << my_money.Info() << '\n' << "Your opponent money:" << opponent_money.Info() << endl;
    }


//delete part

    delete socket;
    delete my_factory;
    delete opponent_factory;
    delete my_city_factory;
    delete opponent_city_factory;
    delete map;

    return 0;
}

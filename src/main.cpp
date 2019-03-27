#include <iostream>
#include "GameSocket.h"

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

    string s;
    getline(cin, s);
    s += '\n';

    GameSocket* sd = new GameSocket();

    sd->Write(s);
    cout << sd->Read();

    delete sd;
    return 0;
}

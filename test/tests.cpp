#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "GameSocket.h"

using std::cin;
using std::cout;
using std::endl;

TEST(GameSocket, server) {
    string s;
    getline(cin, s);
    s += '\n';

    GameSocket* sd = new GameSocket();

    sd->Write(s);
    cout << sd->Read();

    delete sd;
}

TEST(GameSocket, clien) {

    string s;
    getline(cin, s);
    s += '\n';

    GameSocket* sd = new GameSocket("Predator");


    sd->Write(s);
    cout << sd->Read();

    delete sd;
}
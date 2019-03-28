#include <iostream>
#include "GameSocket.h"
#include "UnitFactory.h"

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

    return 0;
}

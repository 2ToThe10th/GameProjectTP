#include <iostream>

#ifdef DEBUG
#include "gtest/gtest.h"
#endif

using std::cin;
using std::cout;
using std::endl;


int main() {
#ifdef DEBUG
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
#endif

    cout << "main" << endl;
    return 0;
}

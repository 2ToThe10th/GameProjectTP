#ifndef GAME_MONEY_H
#define GAME_MONEY_H


#include <string>

class Money {
private:
    static const int START_RESOURCES_AMT = 100500;
    int gold_amt = START_RESOURCES_AMT;
    int iron_amt = START_RESOURCES_AMT;
    int wood_amt = START_RESOURCES_AMT;
public:
    Money() = default;
    ~Money() = default;

    std::string Info();

    void Add( int wood_plus, int iron_plus, int gold_plus);

    bool Take(int wood_minus, int iron_minus, int gold_minus);//true - Ok, false - not enough money
};


#endif //GAME_MONEY_H

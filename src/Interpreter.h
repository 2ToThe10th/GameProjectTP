#ifndef GAME_INTERPRETER_H
#define GAME_INTERPRETER_H

#include <string>
#include "Command/ICommand.h"

using std::string;
using std::vector;

class Interpreter {
private:
    vector<string> Split();
    string ToLowercase(string str);
    int ToInt(string str);
    string command;
public:
    explicit Interpreter(string command);

    ICommand* Translate();
};


#endif //GAME_INTERPRETER_H

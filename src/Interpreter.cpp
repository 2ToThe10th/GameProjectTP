#include <Command/CreateTower.h>
#include "Interpreter.h"

#include "Enums.h"
#include "Command/Attack.h"
#include "Command/BuildCity.h"
#include "Command/CreateUnit.h"
#include "Command/End.h"
#include "Command/Go.h"
#include "Command/Mine.h"
#include "Command/WrongCommand.h"

Interpreter::Interpreter(string command): command(command) {}

vector<string> Interpreter::Split() {

    vector<string> return_string;

    string temp_string;

    for(auto i: command) {
        if(i == ' ') {
            if(!temp_string.empty()) {
                return_string.push_back(temp_string);
                temp_string = "";
            }
        }
        else {
            temp_string += i;
        }
    }

    if(!temp_string.empty()) {
        return_string.push_back(temp_string);
    }

    return return_string;
}

string Interpreter::ToLowercase(string str) {
    for(unsigned int i = 0; i < str.size(); ++i) {
        if((str[i] >= 'A') && (str[i] <= 'Z')) {
            str[i] = char(int(str[i]) - 'A' + 'a');
        }
    }
    return str;
}

int Interpreter::ToInt(string str) {
    int ret = 0;

    for(auto i: str) {
        if(i >= '0' && i <= '9') {
            ret = 10 * ret + int(i - '0');
        }
    }

    return ret;
}

ICommand* Interpreter::Translate() {
    auto split_command = Split();

    if(split_command.empty()) {
        return new WrongCommand();
    }

    string command_name = ToLowercase(split_command[0]);

    if(command_name == "buildcity" || command_name == "bc") {
        if(split_command.size() != 2) {
            return new WrongCommand();
        }
        else {
            unsigned int id_colonist;
            try{
                id_colonist = ToInt(split_command[1]);
            }
            catch(...) {
                return new WrongCommand();
            }

            return new BuildCity(id_colonist);
        }
    }
    else if(command_name == "createunit" || command_name == "cu") {
        if(split_command.size() != 3) {
            return new WrongCommand();
        }
        else {
            unsigned int city_id;
            try{
                city_id = ToInt(split_command[2]);
            }
            catch(...) {
                return new WrongCommand();
            }
            string unit_type = ToLowercase(split_command[1]);
            if(unit_type == "colonist") {
                return new CreateUnit(UnitType::ColonistType, city_id);
            }
            else if(unit_type == "worker") {
                return new CreateUnit(UnitType::WorkerType, city_id);
            }
            else if(unit_type == "warrior") {
                return new CreateUnit(UnitType::WarriorType, city_id);
            }
            else if(unit_type == "archer") {
                return new CreateUnit(UnitType::ArcherType, city_id);
            }
            else if(unit_type == "wizard") {
                return new CreateUnit(UnitType::WizardType, city_id);
            }
            else {
                return new WrongCommand();
            }
        }
    }
    else if(command_name == "createtower" || command_name == "ct") {
        if(split_command.size() != 3) {
            return new WrongCommand();
        }
        else {
            unsigned int city_id;
            try{
                city_id = ToInt(split_command[2]);
            }
            catch(...) {
                return new WrongCommand();
            }
            string tower_type = ToLowercase(split_command[1]);
            if(tower_type == "archer" || tower_type == "a") {
                return new CreateTower(TowerType::ArcherTower, city_id);
            }
            else if(tower_type == "wizard" || tower_type == "w") {
                return new CreateTower(TowerType::WizardTower, city_id);
            }
            else {
                return new WrongCommand();
            }
        }
    }
    else if(command_name == "end" || command_name == "e") {
        if(split_command.size() != 1) {
            return new WrongCommand();
        }
        else {
            return new End();
        }
    }
    else if(command_name == "mine" || command_name == "m") {
        if(split_command.size() != 2) {
            return new WrongCommand();
        }
        else {
            unsigned int worker_id;
            try{
                worker_id = ToInt(split_command[1]);
            }
            catch(...) {
                return new WrongCommand();
            }
            return new Mine(worker_id);
        }
    }
    else if(command_name == "go" || command_name == "g") {
        if(split_command.size() != 4) {
            return new WrongCommand();
        }
        else {
            unsigned int unit_id;
            try{
                unit_id = ToInt(split_command[2]);
            }
            catch(...) {
                return new WrongCommand();
            }
            if(split_command[3].size() != 1) {
                return new WrongCommand();
            }
            Direction where;
            if(ToLowercase(split_command[3]) == "u") {
                where = Direction::Up;
            }
            else if(ToLowercase(split_command[3]) == "d") {
                where = Direction::Down;
            }
            else if(ToLowercase(split_command[3]) == "l") {
                where = Direction::Left;
            }
            else if(ToLowercase(split_command[3]) == "r") {
                where = Direction::Right;
            }
            else {
                return new WrongCommand();
            }
            string unit_type = ToLowercase(split_command[1]);
            if(unit_type == "combat") {
                return new Go(UnitCommonType::CombatCommonType, unit_id, where);
            }
            else if(unit_type == "colonist") {
                return new Go(UnitCommonType::ColonistCommonType, unit_id, where);
            }
            else if(unit_type == "worker") {
                return new Go(UnitCommonType::WorkerCommonType, unit_id, where);
            }
            else {
                return new WrongCommand();
            }
        }
    }
    else if(command_name == "attack" || command_name == "a") {
        if(split_command.size() != 3) {
            return new WrongCommand();
        }
        else {
            unsigned int combat_id;
            try{
                combat_id = ToInt(split_command[1]);
            }
            catch(...) {
                return new WrongCommand();
            }
            
            vector<Direction> where;
            for(auto direction: split_command[2]) {
                if(direction >= 'A' && direction <= 'Z') {
                    direction = char(direction - 'A' + 'a');
                }
                if (direction == 'u') {
                    where.push_back(Direction::Up);
                } else if (direction == 'd') {
                    where.push_back(Direction::Down);
                } else if (direction == 'l') {
                    where.push_back(Direction::Left);
                } else if (direction == 'r') {
                    where.push_back(Direction::Right);
                } else {
                    return new WrongCommand();
                }
            }
            
            return new Attack(combat_id, where);
        }
    }

    return new WrongCommand();
}

#ifndef GAME_GRAPHIC_H
#define GAME_GRAPHIC_H

#include <iostream>
#include <string>
#include "Map.h"

using std::string;
using std::cout;

class Graphic {
private:
    Map* map;
    const string set_red_background_color = "\x1b[41m";
    const string set_green_background_color = "\x1b[42m";
    const string set_yellow_background_color = "\x1b[43m";
    const string set_blue_background_color = "\x1b[44m";
    const string set_magenta_background_color = "\x1b[45m";
    const string set_cyan_background_color = "\x1b[46m";
    const string set_default_background_color = "\x1b[0m";
    const string empty_place = "    ";
public:
    explicit Graphic(Map* map);

    void Draw();
};


#endif //GAME_GRAPHIC_H

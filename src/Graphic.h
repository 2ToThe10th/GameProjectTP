#ifndef GAME_GRAPHIC_H
#define GAME_GRAPHIC_H


#include "Map.h"

class Graphic {
private:
    Map* map;
public:
    explicit Graphic(Map* map);

    void Draw();
};


#endif //GAME_GRAPHIC_H

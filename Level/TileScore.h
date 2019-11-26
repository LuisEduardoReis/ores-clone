//
// Created by luis.reis on 08-04-2018.
//

#ifndef ORESCLONE_TILESCORE_H
#define ORESCLONE_TILESCORE_H


#include "TileType.h"
#include "../Util/Coord.h"

class TileScore {
public:
    explicit TileScore(TileTypeRef tile);

    TileTypeRef tile = TileType::EMPTY;
    int value = 0;
    float slow_value = 0;

    void tick(float delta);
    void render(Graphics &g);

    static Coord getScorePosition(TileTypeRef tile);
};


#endif //ORESCLONE_TILESCORE_H

//
// Created by luis.reis on 08-04-2018.
//

#include "TileScore.h"
#include "../Util/Util.h"

TileScore::TileScore(TileTypeRef tile) : tile(tile) {}

void TileScore::tick(float delta) {
    stepTo(slow_value, (float) value, 10*delta);
}

void TileScore::render(Graphics &g) {
    Coord pos = getScorePosition(tile);

    g.renderTextureRegion(tile->resource_sprite, pos.x, pos.y, 0);
    g.renderText(pos.x + 16, pos.y+4, stringFormat("%d", (int) slow_value));
}

Coord TileScore::getScorePosition(TileTypeRef tile) {
    static unordered_map<TileTypeRef, Coord> positions;
    if (positions.empty()) {
        for(int i = 0; i < TileType::TILES.size(); i++) {
            positions.emplace(&TileType::TILES[i], Coord(i*40, 0));
        }
    }
    if (positions.count(tile) > 0) return positions[tile];
    return Coord(0,0);
}

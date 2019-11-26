//
// Created by luis.reis on 08-04-2018.
//

#include "Resource.h"
#include "../Util/Util.h"
#include "../Level/TileScore.h"
#include "../Level/Level.h"

Resource::Resource(Level &_level, TileTypeRef tile) : Entity(_level), tile(tile) {
    sprite = tile->resource_sprite;
    zIndex = 10;
}

void Resource::tick(float delta) {
    Entity::tick(delta);

    Coord target = TileScore::getScorePosition(tile);

    auto dist = distance<float>(x,y,target.x,target.y);
    float spd = 8*TILE_SIZE;
    vx = spd * (target.x - x) / dist;
    vy = spd * (target.y - y) / dist;

    if (dist < TILE_SIZE/2) {
        remove = true;
        _level.addScore(tile);
    }
}

void Resource::render() {
    if (tile != TileType::EMPTY) Entity::render();
}


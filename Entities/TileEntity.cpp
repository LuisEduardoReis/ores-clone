//
// Created by luis.reis on 01-04-2018.
//

#include "TileEntity.h"
#include "../Level/Level.h"
#include "../Util/Util.h"
#include "../Game.h"
#include "Resource.h"

using namespace std;

TileEntity::TileEntity(Level& _level, TileTypeRef tile) : Entity(_level), type(tile) {
    sprite = tile->ore_sprite;
    gravity = FALL_GRAVITY;
}

TileEntity::~TileEntity() {
    if (!mined) stepTo(_level.lives, 0, 1);
}

void TileEntity::tick(float delta) {
    Entity::tick(delta);

    if (attachedTilePos != Coord::EMPTY) {
        float targetX = attachedTilePos.x * TILE_SIZE + _level.tiles_position.x;
        float targetY = attachedTilePos.y * TILE_SIZE + _level.tiles_position.y;
        stepTo(x, targetX, 32*delta);
        stepTo(y, targetY, vy*delta);
        if (y == targetY) vy = 0;
        if (x == targetX && attachedTilePos.x < 0) TileEntity::drop();
    }
}

void TileEntity::render() {
    if (type != TileType::EMPTY) Entity::render();
}

void TileEntity::click(int, int) {
    if (attachedTilePos != Coord::EMPTY) {
        _level.targetTile = this;
        _level.getTileGroup(attachedTilePos, type, _level.targetTileGroup);
    }
}

void TileEntity::place(int ix, int iy) {
    x = ix * TILE_SIZE + _level.tiles_position.x;
    y = iy * TILE_SIZE + _level.tiles_position.y;
}

void TileEntity::mine() {
    if (type == TileType::EMPTY) return;

    // Shoot in a random direction
    vx = random(-2*TILE_SIZE, 2*TILE_SIZE);
    vy = -random(8*TILE_SIZE);
    vangle = 3*random(-PI,PI);

    // Create resource
    auto resource = _level.addEntity<Resource>(_level, type);
    resource->setPosition(x,y);
    float v = 4*TILE_SIZE;
    float dir = random(-PI,PI);
    resource->setForce(v*cos(dir),v*sin(dir));
    resource->efric = v;

    // Set as mined
    mined = true;

    drop();
}

void TileEntity::drop() {
    zIndex = 1;
    attachedTilePos = Coord::EMPTY;
    gravity = DROP_GRAVITY;
}

void TileEntity::check() {
    if (attachedTilePos == Coord::EMPTY) return;
    if (!between2d(attachedTilePos.x, attachedTilePos.y, 0,0, _level.tiles_w-1,_level.tiles_h-1)) return;
    if (_level.getTile(attachedTilePos) == this) return;

    cout << "Error with tile: " << this << endl;
}

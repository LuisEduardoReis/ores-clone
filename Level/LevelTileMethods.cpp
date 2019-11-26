//
// Created by luis.reis on 08-04-2018.
//

#include <queue>
#include "./Level.h"

using namespace std;

void Level::getTileGroup(const Coord &target, TileTypeRef targetTile, vector<TileEntity*> &result) {
    result.clear();
    if (targetTile == TileType::EMPTY) return;

    queue<Coord> queue;
    set<Coord> visited;

    visited.insert(target);
    queue.push(target);
    result.push_back(getTile(target));

    while(!queue.empty()) {
        Coord n = queue.front(); queue.pop();
        Coord neighbours[] = {Coord(n.x-1,n.y), Coord(n.x+1,n.y), Coord(n.x,n.y-1), Coord(n.x,n.y+1)};
        for(auto neigh : neighbours) {
            if (visited.find(neigh) == visited.end() && getTile(neigh.x, neigh.y)->type == targetTile) {
                visited.insert(neigh);
                queue.push(neigh);
                result.push_back(getTile(neigh));
            }
        }
    }
}

TileEntity* Level::createTile(int ix, int iy, TileTypeRef tile) {
    auto newTile = addEntity<TileEntity>(*this, tile);
    setTile(ix, iy, newTile);
    newTile->place(ix,iy);
    return newTile;
}

TileEntity* Level::getTile(const Coord &coord) const {
    return getTile(coord.x, coord.y);
}

TileEntity* Level::getTile(int x, int y) const {
    if (x < 0 || y < 0 || x >= tiles_w || y >= tiles_h) return empty_tile;
    return tiles[y * tiles_w + x];
}

void Level::setTile(int x, int y, TileEntity* tile) {
    if (x < 0 || y < 0 || x >= tiles_w || y >= tiles_h) return;
    tiles[y * tiles_w + x] = tile;
}

void Level::setTile(const Coord &coord, TileEntity* tile) {
    setTile(coord.x, coord.y, tile);
}

void Level::moveTile(TileEntity* tile, int x, int y) {
    if (tile == empty_tile || tile->type == TileType::EMPTY) return;
    setTile(tile->attachedTilePos, empty_tile);
    setTile(x, y, tile);
    tile->attachedTilePos.set(x,y);
}

bool Level::columnIsEmpty(int column) {
    if (column < 0 || column >= tiles_w) return true;

    for(int y = 0; y < tiles_h; y++) {
        if (getTile(column, y) != empty_tile) return false;
    }

    return true;
}
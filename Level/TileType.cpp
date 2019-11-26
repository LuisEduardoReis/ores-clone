//
// Created by luis.reis on 27-03-2018.
//

#include <memory>
#include "TileType.h"
#include "../Util/Util.h"

using namespace std;

TileType::TileType(const std::string& name, SDL_Texture *texture, int tx, int ty) : name(name) {
    ore_sprite = TextureRegion(texture, tx * TILE_SIZE, ty * TILE_SIZE);
    resource_sprite = TextureRegion(texture, tx * TILE_SIZE, (ty+1) * TILE_SIZE);
}

vector<TileType> TileType::TILES;
unordered_map<string, TileTypeRef> TileType::TILES_MAP;

TileType TileType::_EMPTY;
TileTypeRef TileType::EMPTY = &TileType::_EMPTY;

void TileType::initTiles(Graphics& graphics) {

    SDL_Texture* texture = graphics.getTextureByName("tileset");
    TILES.push_back(TileType("Diamond", texture, 2, 0));
    TILES.push_back(TileType("Ruby", texture, 3, 0));
    TILES.push_back(TileType("Gold", texture, 4, 0));
    TILES.push_back(TileType("Iron", texture, 5, 0));

    for (auto &tile : TILES) {
        TILES_MAP.emplace(tile.name, &tile);
    }
    TILES_MAP.emplace("Empty", EMPTY);
}


TileTypeRef TileType::getTileByName(const std::string &name) {
    if (TILES_MAP.find(name) == TILES_MAP.end()) return EMPTY;
    return TILES_MAP[name];
}

TileTypeRef TileType::getRandomTile() {
    return &TILES[random((int) TILES.size())];
}

bool TileType::operator==(const TileType& rhs) const {
    return name == rhs.name;
}

bool TileType::operator!=(const TileType& rhs) const {
    return !(rhs == *this);
}

//
// Created by luis.reis on 27-03-2018.
//

#ifndef ORESCLONE_TILE_H
#define ORESCLONE_TILE_H

#include <utility>
#include <vector>
#include "../Graphics/Graphics.h"

class TileType;
typedef const TileType* TileTypeRef;

/**
 * "Enum" class that defines all types of tile.
 */
class TileType {

private:
    static TileType _EMPTY;
    TileType() : TileType("Empty", nullptr, 0,0) {};
    TileType(const std::string& name, SDL_Texture* texture, int tx, int ty);

public:
    std::string name = "";
    TextureRegion ore_sprite;
    TextureRegion resource_sprite;

    static std::unordered_map<std::string, TileTypeRef> TILES_MAP;
    static std::vector<TileType> TILES;
    static TileTypeRef EMPTY;
    static TileTypeRef getRandomTile();

    static void initTiles(Graphics& graphics);
    static TileTypeRef getTileByName(const std::string &name);

    bool operator==(const TileType& rhs) const;
    bool operator!=(const TileType& rhs) const;
};



class UnknownTileException {};

#endif //ORESCLONE_TILE_H

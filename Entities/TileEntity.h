//
// Created by luis.reis on 01-04-2018.
//

#ifndef ORESCLONE_TILEENTITY_H
#define ORESCLONE_TILEENTITY_H


#include <memory>
#include <utility>
#include "../Level/TileType.h"
#include "../Util/Coord.h"
#include "Entity.h"

class Level;

static const int DROP_GRAVITY = 24 * TILE_SIZE;
static const int FALL_GRAVITY = 12*TILE_SIZE;

/**
 * Class that represents a tile, both in the puzzle logic and visually.
 */
class TileEntity : public Entity {

public:

    TileEntity(Level& _level, TileTypeRef tile);
    ~TileEntity() override;

    void tick(float delta) override;
    void render() override;
    void click(int mouse_x, int mouse_y) override;

    /************************************************************
     *                        OVERRIDES                         *
     ************************************************************/

    /**
     * Type of the tile represented.
     */
    TileTypeRef type = TileType::EMPTY;

    /**
     * Backreference to the matrix position this tile is attached to.
     * May not represent the actual position of the tile or even
     * a position inside the matrix.
     * Coord::EMPTY means the tile is detached (e.g. affected by gravity).
     */
    Coord attachedTilePos = Coord::EMPTY;

    /**
     * Whether the tile was mined.
     */
    bool mined = false;

    /**
     * Places the tile in the corresponding position of the tile matrix.
     * @param ix x index in the matrix.
     * @param iy y index in the matrix.
     */
    void place(int ix, int iy);

    /**
     * Mines the tile.
     * - Shoots it in a random direction.
     * - Creates a resource entity that will increment the score.
     * - Sets the "mined" flag.
     * - Drops it. \see TileEntity::drop().
     */
    void mine();

    /**
     * Drops the tile.
     * - Moves it up in the zIndex.
     * - Detaches it from the matrix (sets the backreference only).
     * - Sets its gravity to drop gravity.
     */
    void drop();

    /**
     * Debug function to check the tile is valid.
     */
    void check() override;
};

#endif //ORESCLONE_TILEENTITY_H

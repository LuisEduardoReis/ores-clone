//
// Created by luis.reis on 27-03-2018.
//

#ifndef ORESCLONE_LEVEL_H
#define ORESCLONE_LEVEL_H

#include <vector>
#include <set>
#include <map>
#include <memory>
#include "../Graphics/Graphics.h"
#include "TileType.h"
#include "../Util/Coord.h"
#include "../Entities/TileEntity.h"
#include "TileScore.h"
#include "../Constants.h"

class Game;

/**
 * State of the game and abstraction for all game logic.
 */
class Level {

private:
    /**
     * Instance of an empty tile (null object pattern).
     * Needs to exist per level as all entities only exist in the context of a level.
     * \see Level::empty_tile
     */
    std::unique_ptr<TileEntity> _empty_tile;

public:
    /**
     * Backreference to the game this level belongs to.
     */
    Game& game;

    /**
     * Time elapsed since the start of the level.
     */
    float t = 0;

    /**
     * Lives. Number of the tiles the player can drop before losing.
     */
    int lives = DEFAULT_STARTING_LIVES;

    /**
     * Time since the player lost. -1 when the game is still going.
     */
    float lose_timer = -1;

    /************************************************************
     *                          TILES                           *
     ************************************************************/

    /**
     * Bounding box of the tiles in the level.
     */
    SDL_Rect tiles_position;

    /**
     * Size of the tile matrix.
     */
    const int tiles_w, tiles_h;

    /**
     * Tile matrix. Authoritative state of the puzzle at all times.
     * Consists of weak references to tiles which are owned by the Level::entities vector.
     * Tiles may be "detached" from this matrix for the purposes of animation before being deleted.
     */
    std::vector<TileEntity*> tiles;

    /**
     * \see Level::_empty_tile
     */
    TileEntity* empty_tile;

    /**
     * Storage with ownership of all entities in the game.
     * These are ticked and rendered in the order they are listed here.
     * They will be reordered by zIndex in every tick.
     */
    std::vector<std::unique_ptr<Entity>> entities;

    /**
     * Push event.
     * push_timer   - Time since the last tile push event in seconds.
     * push_delay   - Time it takes for a push event to happen in seconds.
     */
    float push_timer = 0, push_delay = DEFAULT_PUSH_DELAY;


    /************************************************************
     *                         MINING                           *
     ************************************************************/

    /**
     * State of the mining
     * targetTile       - Tile that is currently being mined. Selected on mouse left down.
     * targetTileGroup  - Group of tiles that will be mined along with the current one.
     * miningTime       - Time spent mining so far in seconds.
     *                       0  - Ready to mine.
     *                      >0  - Mining.
     *                      -1  - Mined. Reset to 0 at mouse left up.
     * miningDelay      - Time it takes to mine in seconds.
     * miningAnimation  - Array of sprites with successive states of "cracks" displayed on tiles being mined.
     * score            - Score for each tile type. \see TileScore.
     */
    TileEntity* targetTile = nullptr;
    std::vector<TileEntity*> targetTileGroup;
    float miningTime = 0, miningDelay = DEFAULT_MINING_DELAY;
    std::vector<TextureRegion> miningAnimation;
    std::map<TileTypeRef, TileScore> score;


    /************************************************************
     *                         METHODS                          *
     ************************************************************/

    /**
     * Constructor for Level.
     * Initializes tile matrix position, size and contents (with random tiles)
     * Initializes the score and mining animation sprites.
     *
     * @param game Backreference to the Game this level belongs to.
     */
    explicit Level(Game& game);

    /**
     * Tick event for the level.
     *
     * @param delta Time passed since the last tick in seconds.
     */
    void tick(float delta);

    /**
     * Tile logic auxiliary function. Used in Level::tick().
     *
     * @param delta Time passed since the last tick in seconds.
     */
    void tileLogic(float delta);

    /**
     * Renders the level.
     */
    void render();

    /**
     * Click event for the level.
     *
     * @param x x coordinate of the mouse in virtual window coordinates.
     * @param y y coordinate of the mouse in virtual window coordinates.
     */
    void click(int x, int y);

    /**
     * Creates a new tile of a certain type in the tile matrix.
     * Creates the tile entity, adds it to the matrix and sets its position
     * to the correct position in the matrix.
     *
     * @param ix x index in the tile matrix.
     * @param iy y index in the tile matrix.
     * @param tile Type of the tile to create.
     * @return A raw pointer to the created tile.
     */
    TileEntity* createTile(int ix, int iy, TileTypeRef tile);

    /**
     * Getters for a tile in the matrix.
     *
     * @param coord Coordinate of the tile.
     * @return
     *      TileEntity::EMPTY if the coord is outside the matrix.
     *      The tile at the given position otherwise.
     */
    TileEntity* getTile(const Coord& coord) const;
    TileEntity* getTile(int x, int y) const;

    /**
     * Setters for a tile in the matrix.
     * If the given position is outside the matrix it does nothing.
     *
     * @param coord Coordinate of the tile.
     * @param tile Tile to place.
     */
    void setTile(const Coord& coord, TileEntity* tile);
    void setTile(int x, int y, TileEntity* t);

    /**
     * Calculates the tiles that will be broken along with a given tile
     * via a flood fill. Includes the source tile.
     *
     * @param target Position of the broken tile.
     * @param targetTile Type of the broken tile.
     * @param result Reference to the vector to fill with the results.
     */
    void getTileGroup(const Coord& target, TileTypeRef targetTile, std::vector<TileEntity*>& result);

    /**
     * Moves a tile from its position in the matrix to another.
     * - If the tile is empty, does nothing.
     * - Empties the matrix position the tile was in.
     * - Places the tile in the new position.
     * - Updates the tile's backreference to its position in the matrix.
     *
     * @param tile The tile to move.
     * @param ix
     * @param iy
     */
    void moveTile(TileEntity* tile, int ix, int iy);

    /**
     * Checks whether a column in the matrix is empty.
     *
     * @param column The column to test.
     * @return Whether the column is empty.
     */
    bool columnIsEmpty(int column);

    /**
     * Adds a point to the score of a given tile type.
     *
     * @param tile Type of the score to increment.
     */
    void addScore(TileTypeRef tile);

    /**
     * Adds a new entity to the level.
     * This creates an unique_ptr for the entity that will be freed with the level,
     * or when the entity is removed.
     *
     * @tparam Type Class of the entity to create.
     * @tparam Args Argument package template.
     * @param args Arguments to pass to the constructor of the entity.
     * @return Raw pointer to the created entity.
     */
    template<class Type, class ... Args>
    Type* addEntity(Args& ... args) {
        entities.push_back(std::make_unique<Type>(args ...));
        return (Type*) entities.back().get();
    }
};


#endif //ORESCLONE_LEVEL_H

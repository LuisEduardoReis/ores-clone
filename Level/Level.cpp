//
// Created by luis.reis on 27-03-2018.
//

#include <algorithm>
#include <set>
#include "Level.h"
#include "../Util/Util.h"
#include "../Game.h"

using namespace std;

Level::Level(Game& game) : game(game), tiles_w(DEFAULT_TILES_W), tiles_h(DEFAULT_TILES_H) {
    Graphics& g = *game.graphics;

    _empty_tile = make_unique<TileEntity>(*this, TileType::EMPTY);
    empty_tile = _empty_tile.get();
    targetTile = empty_tile;

    // Tiles size and bounding box
    tiles_position = {
            x: (SCREEN_WIDTH - tiles_w*TILE_SIZE) / 2,
            y: (SCREEN_HEIGHT - tiles_h*TILE_SIZE) / 2,
            w: tiles_w * TILE_SIZE,
            h: tiles_h * TILE_SIZE
    };

    // Initial tile board
    tiles.assign((size_t) tiles_w * tiles_h, empty_tile);
    for (int iy = 0; iy < tiles_h; iy++) {
        for(int ix = tiles_w - DEFAULT_STARTING_COLUMNS; ix < tiles_w; ix++) {
            createTile(ix, iy, TileType::getRandomTile());
        }
    }

    // Score for each tile type
    for (auto &tile : TileType::TILES) {
        score.emplace(&tile, TileScore(&tile));
    }

    // Mining animation sprites
    for(int i = 0; i < 10; i++) miningAnimation.push_back(g.getTileSprite(i, 2));
}

void Level::tick(float delta) {
    t += delta;

    /*****  Tile/Game logic  *****/

    if (lives > 0) {
        tileLogic(delta);
    } else if (lose_timer < 0) {
        lose_timer = 0;
        // Lose event
    } else {
        lose_timer += delta;
    }


    /***** Entity logic *****/

    // Sort entities by z-index
    stable_sort(entities.begin(), entities.end(), Entity::zIndexComparator<unique_ptr<Entity>>);

    // Tick all entities
    for(auto& entity : entities) {
        entity->tick(delta);
    }

    // Remove old entities
    entities.erase(
            remove_if(entities.begin(), entities.end(),
                      [](const unique_ptr<Entity>& tileEntity){ return tileEntity->remove; }),
            entities.end()
    );

    // Score
    for(auto &s : score) s.second.tick(delta);
}

void Level::tileLogic(float delta) {

    // Mining
    if (game.input->mouse_left) {
        if (miningTime >= 0)
            miningTime += delta;

        if (miningTime > miningDelay) {

            if (targetTile != empty_tile) {
                for (auto& tile : targetTileGroup) {
                    setTile(tile->attachedTilePos, empty_tile);
                    tile->mine();
                }
                targetTile = empty_tile;
                targetTileGroup.clear();
            }
            miningTime = -1;
        }
    } else {
        miningTime = 0;
    }

    // Push
    int firstEmptyColumn;
    push_timer += delta;
    if (push_timer > push_delay) {

        // Find first empty column from the right
        for(firstEmptyColumn = tiles_w - 1; !columnIsEmpty(firstEmptyColumn); firstEmptyColumn--);

        // Detach ores on left-most column in case they were pushed
        if (firstEmptyColumn < 0) {
            for(int y = 0; y < tiles_h; y++) {
                auto tileEntity = getTile(0, y);
                if (tileEntity != empty_tile) {
                    moveTile(tileEntity, -1, y);
                    tileEntity->zIndex = 1;
                }
            }
        }

        // Push all columns after the first empty column to the left
        for (int x = max(firstEmptyColumn+1,1); x < tiles_w; x++) {
            for(int y = 0; y < tiles_h; y++) {
                moveTile(getTile(x, y), x - 1, y);
            }
        }

        // Add new column on the right
        for(int y = 0; y < tiles_h; y++) {
            createTile(tiles_w - 1, y, TileType::getRandomTile())->place(tiles_w, y);
        }

        // Reset timer
        push_timer = 0;
    }

    // Falling tiles

    for(int column = 0; column < tiles_w; column++) {

        // Get tiles that are not empty
        vector<TileEntity*> solidTiles;
        for(Coord coord = Coord(column, tiles_h - 1); coord.y >= 0; coord.y--) {
            if (getTile(coord)->type != TileType::EMPTY)
                solidTiles.push_back(getTile(coord));
        }

        // Place tiles at the bottom
        for(int i = 0; i < tiles_h; i++) setTile(column, i, empty_tile);
        for(int i = 0; i < solidTiles.size(); i++) {
            moveTile(solidTiles[i], column, tiles_h - 1 - i);
        }
    }

    for(auto& e : entities) e->check();
}

void Level::render() {
    Graphics& g = *game.graphics;

    //g.renderTexture("hello", (int)(100 + 50 * sin(t)), (int)(100 + 50 * cos(t)));

    // Tiles
    if (game.input->mouse_right) {
        for (int i = 0; i < tiles_h; i++) {
            for (int j = 0; j < tiles_w; j++) {
                TileTypeRef tile = getTile(j, i)->type;
                g.renderTextureRegion(tile->ore_sprite, j * TILE_SIZE + tiles_position.x, i * TILE_SIZE + tiles_position.y, 0);
            }
        }
    } else {
        for(auto& entity : entities) {
            entity->render();
        }
    }

    // Mining
    if (miningTime > 0) {
        g.setColor(0xFFFFFF, 0);
        auto l = (int)((miningDelay - miningTime) / miningDelay * 16);
        g.rect(game.input->mouse_x,game.input->mouse_y, l,4);

        for(auto &tile : targetTileGroup) {
            int animState = clamp(0, (int) (miningAnimation.size() * miningTime / miningDelay), (int) miningAnimation.size() - 1);
            g.renderTextureRegion(miningAnimation[animState], (int) tile->x, (int) tile->y, tile->angle);
        }
    }

    // Base
    TextureRegion stone = g.getTileSprite(1,1);
    for(int i = 0; i < tiles_w; i++) {
        g.renderTextureRegion(stone,tiles_position.x + i*TILE_SIZE,tiles_position.y + tiles_position.h);
    }
    for(int i = 0; i <= tiles_h; i++) {
        g.renderTextureRegion(stone,tiles_position.x + tiles_position.w,tiles_position.y + i*TILE_SIZE);
    }

    // Lose screen
    if (lose_timer > 0) {
        float blackout_delay = 0.5f;
        float blackout_duration = 1.5f;
        auto blackout_alpha = clamp<float>(0, (lose_timer-blackout_delay) / blackout_duration, 1) * 0.75f;

        g.setColor(0x0, blackout_alpha);
        g.rect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
        g.setColor(0xFFFFFFFF, 0);

        int gameouver_y_anim = lerp(-TILE_SIZE, SCREEN_HEIGHT/3, (lose_timer - blackout_duration)/1);
        g.renderText((int)(SCREEN_WIDTH/2 - 4.25*8), gameouver_y_anim, "GAME OVER");

        float flash_delay = 1.5f;
        if (lose_timer > 4 && fmod(lose_timer,flash_delay) < flash_delay/2) {
            g.renderText(SCREEN_WIDTH/2 - 8*8, SCREEN_HEIGHT/2, "Press R to restart.");
        }
    }

    // Score and lives
    for (auto &s : score) s.second.render(g);

    TextureRegion heart = g.getTileSprite(0,1);
    g.renderTextureRegion(heart, SCREEN_WIDTH - 40, 0);
    g.renderText(SCREEN_WIDTH - 24, 4, stringFormat("%d", lives));
}

void Level::click(int mx, int my) {
    for(auto& e : entities) {
        if (between2d((float) mx,(float) my, e->x,e->y, e->x+e->w, e->y+e->h))
            e->click(mx,my);
    }
}

void Level::addScore(TileTypeRef tile) {
    if (score.count(tile) == 0) return;

    stepTo(score.at(tile).value, 999, 1);
}

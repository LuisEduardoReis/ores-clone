//
// Created by luis.reis on 25-03-2018.
//

#ifndef ORESCLONE_GAME_H
#define ORESCLONE_GAME_H

#include <memory>
#include "Graphics/Graphics.h"
#include "Level/Level.h"
#include "Input.h"

/**
 * Main class of the game.
 * Contains initializations of SDL and SDL_IMAGE.
 * Owns Graphics, Input and Level objects.
 */
class Game {

private:
    /**
     * Whether the game is running. Set to false to stop the game.
     * If the game init fails this value is never set to true and it immediately terminate.
     */
    bool _running = false;

    /**
     * Number of milliseconds since the game started when the last tick occurred.
     * Obtained through SDL_GetTicks()
     * Used to calculate how much time passed since the last frame.
     */
    Uint32 _currentTick = UINT32_MAX;

    /**
     * FPS counter state.
     * _frameCount      Number of frames that were rendered since the last fps count update.
     * _fpsAccumulator  Total time that took to tick/render all the frames since the last fps count update.
     * _fps             Last calculated value of the Frames Per Second of the game.
     */
    Uint32 _frameCount = 0;
    float _fpsAccumulator = 0;
    int _fps = 0;

public:
    /**
     * Graphics wrapper around SDL api for rendering.
     * \see Graphics.
     */
    std::unique_ptr<Graphics> graphics;

    /**
     * Object that handles input events.
     * \see Input.
     */
    std::unique_ptr<Input> input;

    /**
     * Level object contains game state and abstracts game logic.
     * Also allows for multiple levels to be implemented later.
     * \see Level.
     */
    std::unique_ptr<Level> level;

public:

    /**
     * Getter for _running. \see Game::_running.
     * @return Whether the game is running.
     */
    bool isRunning() { return _running; }

    /**
     * Stops the game after finishing the current game loop iteration.
     */
    void stop() { _running = false; }


    /**
     * Restarts the game.
     */
    void restart();

    /**
     * Constructor for Game.
     * Initializes Graphics, Input and Level objects.
     * Initializes the TileType Enum class.
     *
     * If any of the initializations fails the constructor returns without
     * setting _running to true, terminating the game immediately.
     */
    Game();

    /**
     * Game tick.
     * - Updates FPS counter state.
     * - Updates current tick and calculates how much time has passed since
     *   the last tick (delta), capped to 1s.
     * - Calls current level tick with calculated delta.
     */
    void tick();

    /**
     * Render a game frame.
     * - Clears the screen. \see Graphics::clear().
     * - Renders the level.
     * - Renders debug info.
     * - Presents the frame on screen. \see Graphics::present().
     */
    void render();
};

#endif

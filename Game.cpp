//
// Created by luis.reis on 25-03-2018.
//

#include <cmath>
#include <iostream>
#include "Game.h"
#include "Util/Util.h"

using namespace std;

Game::Game() {

    graphics = make_unique<Graphics>();
    if (graphics->init() != 0) return;

    TileType::initTiles(*graphics);

    input = make_unique<Input>(*this);
    restart();

    _currentTick = SDL_GetTicks();
    _running = true;

}

void Game::restart() {
    level = make_unique<Level>(*this);
}


void Game::tick() {
    // Calculate delta and update _currentTick
    auto delta = ((float)(SDL_GetTicks() - _currentTick))/1000;
    delta = min(delta, 1.0f);
    _currentTick = SDL_GetTicks();

    // Update fps counter
    _frameCount++;
    _fpsAccumulator += delta;
    if (_frameCount % 60 == 0) {
        _fps = (int) std::round(60/_fpsAccumulator);
        _fpsAccumulator = 0;
    }

    // Do tick
    if (level) level->tick(delta);
}

void Game::render() {
    graphics->clear();

    level->render();
    graphics->renderText(0,SCREEN_HEIGHT-24,stringFormat("Entities: %d\nFPS: %d", level->entities.size(), _fps));

    graphics->present();
}
//
// Created by luis.reis on 08-04-2018.
//
#include "Resource.h"
#include "../Util/Util.h"
#include "../Level/Level.h"
#include "../Game.h"

void Entity::tick(float delta) {
    t += delta;
    x += (vx + ex) * delta;
    y += (vy + ey) * delta;
    vy += gravity*delta;
    angle += vangle*delta;
    stepTo(ex, 0.0f, efric*delta);
    stepTo(ey, 0.0f, efric*delta);

    int b = 2*TILE_SIZE;
    if (x < -b || y < -b || x > SCREEN_WIDTH+b || y > SCREEN_HEIGHT+b) remove = true;
}

void Entity::render() {
    Graphics& g = *_level.game.graphics;

    g.renderTextureRegion(sprite, (int) roundf(x), (int) roundf(y), angle);
}
//
// Created by luis.reis on 08-04-2018.
//

#ifndef ORESCLONE_ENTITY_H
#define ORESCLONE_ENTITY_H


#include <memory>
#include <utility>
#include <SDL_rect.h>
#include "../Constants.h"
#include "../Graphics/TextureRegion.h"

class Level;

class Entity {

protected:
    Level& _level;

public:
    TextureRegion sprite{};
    float t = 0;
    float x = 0, y = 0;
    float vx = 0, vy = 0;
    float ex = 0, ey = 0;
    float efric = TILE_SIZE;
    float h = TILE_SIZE, w = TILE_SIZE;
    int zIndex = 0;
    float angle = 0;
    float vangle = 0;
    float gravity = 0;
    bool remove = false;

    explicit Entity(Level& _level) : _level(_level) {}
    virtual ~Entity() = default;;

    virtual void tick(float delta);
    virtual void render();
    virtual void click(int mouse_x, int mouse_y) {};

    virtual void check() {};

    void setPosition(float _x, float _y) { x = _x; y = _y; }
    void setVelocity(float _vx, float _vy) { vx = _vx; vy = _vy; };
    void setForce(float dir, float val) { ex = val*cosf(dir); ey = val*sinf(dir); };

    template<typename T>
    static bool zIndexComparator(const T& a, const T& b){ return a->zIndex < b->zIndex; }


};


#endif //ORESCLONE_ENTITY_H

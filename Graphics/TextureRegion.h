//
// Created by luis.reis on 31-03-2018.
//

#include <SDL_system.h>
#include "../Constants.h"

#ifndef ORESCLONE_TEXTUREREGION_H
#define ORESCLONE_TEXTUREREGION_H

class TextureRegion {

public:
    SDL_Texture* texture = nullptr;
    SDL_Rect srcrect{};

    TextureRegion() : TextureRegion(nullptr, 0,0, TILE_SIZE, TILE_SIZE) {}
    TextureRegion(SDL_Texture* _texture, int x, int y, int w = TILE_SIZE, int h = TILE_SIZE) : texture(_texture) {
        srcrect.x = x;
        srcrect.y = y;
        srcrect.w = w;
        srcrect.h = h;
    }
};

#endif //ORESCLONE_TEXTUREREGION_H

//
// Created by luis.reis on 07-04-2018.
//

#ifndef ORESCLONE_FONT_H
#define ORESCLONE_FONT_H


#include <vector>
#include <iostream>
#include <SDL_render.h>
#include "TextureRegion.h"

class Graphics;

class Font {

private:
    Graphics& graphics;
    SDL_Texture *_texture;
    std::vector<TextureRegion> chars;

public:
    explicit Font(Graphics &graphics, std::string texture);

    void text(int x, int y, std::string text);

    void debug();
};


#endif //ORESCLONE_FONT_H

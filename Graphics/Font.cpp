//
// Created by luis.reis on 07-04-2018.
//

#include "Font.h"
#include "Graphics.h"

Font::Font(Graphics &graphics, std::string texture) : graphics(graphics) {

    _texture = graphics.getTextureByName(texture);

    int char_w = 16;
    int char_h = 11;
    int char_spacing_x = 16;
    int char_spacing_y = 12;
    for(int iy = 0; iy < 16; iy++) {
    for(int ix = 0; ix < 16; ix++) {
        chars.emplace_back(_texture, ix*char_spacing_x, iy*char_spacing_y, char_w, char_h);
    }}
}

void Font::text(int x, int y, std::string text) {

    int cx = 0, cy = 0;
    for (int c : text) {
        if (c < 0 || c >= 256) continue;
        if (c == '\n') {
            cx = 0; cy += 12;
            continue;
        }
        if (c == ' ') {
            cx += 4;
            continue;
        }

        graphics.renderTextureRegion(chars[c], x + cx, y + cy, 0);
        cx += 8;
    }
}

void Font::debug() {
    for(int iy = 0; iy < 16; iy++)
    for(int ix = 0; ix < 16; ix++) {
        graphics.renderTextureRegion(chars[ix + iy * 16], ix * 16, iy * 16 + 30, 0);
    }
}

//
// Created by luis.reis on 25-03-2018.
//

#include <SDL_image.h>
#include "Graphics.h"
#include "../Util/Util.h"

using namespace std;

int Graphics::init() {

    // Init
    _sdlInit = (SDL_Init(SDL_INIT_EVERYTHING) == 0);
    if (!_sdlInit) {
        logSDLError(cout, "SDL_Init");
        return 1;
    }

    // Window
    int scale = 3;
    _window = SDL_CreateWindow("Ores Clone",
                               100, 100, scale * SCREEN_WIDTH, scale *SCREEN_HEIGHT,
                               SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (_window == nullptr) {
        logSDLError(cout, "SDL_CreateWindow");
        return 2;
    }

    // Image lib
    int imgFlags = IMG_INIT_PNG;
    _imgInit = (IMG_Init(imgFlags) & imgFlags) != 0;
    if (!_imgInit) {
        logSDLImageError(cout, "IMG_Init");
        return 3;
    }

    // Renderer
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    if (_renderer == nullptr) {
        logSDLError(cout, "SDL_CreateRenderer");
        return 4;
    }
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

    // Virtual Window and viewport
    _virtualWindow = SDL_CreateTexture( _renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (_virtualWindow == nullptr) {
        logSDLError(cout, "SDL_CreateTexture _virtualWindow");
        return 5;
    }
    SDL_GetWindowSize(_window, &window_width, &window_height);
    updateViewport(window_width, window_height);

    // Textures and Font
    if (loadTextures() != 0) return 6;
    _font = make_unique<Font>(*this, "chars");

    // Tileset
    for (int iy = 0; iy < 16; iy++) {
    for (int ix = 0; ix < 16; ix++) {
        tileset.emplace_back(getTextureByName("tileset"), ix*TILE_SIZE,iy*TILE_SIZE, TILE_SIZE,TILE_SIZE);
    }}

    return 0;
}

Graphics::~Graphics() {
    if (_sdlInit) SDL_Quit();
    if (_imgInit) IMG_Quit();
    if (_window != nullptr) SDL_DestroyWindow(_window);
    if (_renderer != nullptr) SDL_DestroyRenderer(_renderer);
    destroyTextures();
}

int Graphics::loadTextures() {

    _textures.emplace("hello", loadTexture("res/hello.bmp"));
    _textures.emplace("tileset", loadTexture("res/tileset.png"));
    _textures.emplace("chars", loadTexture("res/chars.png"));

    for (auto texture : _textures) {
        if (texture.second == nullptr) {
            logSDLError(cout, "SDL_CreateTexture loadTextures");
            return 1;
        }
    }
    return 0;
}

void Graphics::destroyTextures() {
    for (auto &texture : _textures) {
        SDL_DestroyTexture(texture.second);
    }
}


SDL_Texture *Graphics::loadTexture(const std::string &file) {
    SDL_Texture *texture = nullptr;
    SDL_Surface *loadedImage = IMG_Load(file.c_str());

    if (loadedImage == nullptr) {
        logSDLError(std::cout, "IMG_Load");
        return nullptr;
    }

    texture = SDL_CreateTextureFromSurface(_renderer, loadedImage);
    SDL_FreeSurface(loadedImage);

    if (texture == nullptr) {
        logSDLError(std::cout, "CreateTextureFromSurface");
    }

    return texture;
}

SDL_Texture * Graphics::getTextureByName(const std::string &name)const {
    if (_textures.count(name) > 0) return _textures.at(name);
    return nullptr;
}

void Graphics::renderTextureRegion(const TextureRegion &textureRegion, int x, int y, float angle) const {
    if (textureRegion.texture == nullptr) return;

    SDL_Rect dst{ x: x, y: y, w: TILE_SIZE, h: TILE_SIZE};
    dst.w = textureRegion.srcrect.w;
    dst.h = textureRegion.srcrect.h;
    SDL_RenderCopyEx(_renderer, textureRegion.texture, &textureRegion.srcrect, &dst, angle * RADTODEG, nullptr, SDL_FLIP_NONE);
}

void Graphics::clear() {
    // Set render target to and clear virtual window
    SDL_SetRenderTarget(_renderer, _virtualWindow);
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(_renderer);
}

void Graphics::present() {
    // Set render target to and clear actual window
    SDL_SetRenderTarget(_renderer, nullptr);
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(_renderer);

    // Copy virtual window to actual window and present frame
    SDL_RenderCopy(_renderer, _virtualWindow, nullptr, &_viewport);
    SDL_RenderPresent(_renderer);
}

void Graphics::windowToGameCoords(int wx, int wy, int &x, int &y)const {
    // Get window size
    int ww, wh; SDL_GetWindowSize(_window, &ww, &wh);

    // Remove black borders
    wx -= _viewport.x;
    wy -= _viewport.y;

    // Scale to game coords
    x = wx * SCREEN_WIDTH / _viewport.w;
    y = wy * SCREEN_HEIGHT / _viewport.h;
}

void Graphics::updateViewport(int ww, int wh) {
    window_width = ww;
    window_height = wh;

    float dst_ratio = ((float) window_width)/window_height;
    float src_ratio =  ((float) SCREEN_WIDTH)/SCREEN_HEIGHT;
    if (dst_ratio > src_ratio) {
        _viewport.h = window_height;
        _viewport.w = (int)(window_height * src_ratio);
        _viewport.x = (window_width - _viewport.w) / 2;
        _viewport.y = 0;

    } else {
        _viewport.w = window_width;
        _viewport.h = (int)(window_width / src_ratio);
        _viewport.y = (window_height - _viewport.h) / 2;
        _viewport.x = 0;
    }
}

void Graphics::setColor(int color, float alpha) {
    alpha = clamp<float>(0,alpha,1);
    SDL_SetRenderDrawColor(_renderer,
                           (Uint8)((color & 0xFF0000) >> 16),
                           (Uint8)((color & 0xFF00) >> 8),
                           (Uint8)(color & 0xFF),
                           static_cast<Uint8>(0xFF * alpha));
}

void Graphics::rect(int x, int y, int w, int h) {
    SDL_Rect fillRect{ x:x,y:y, w:w,h:h };
    SDL_RenderFillRect(_renderer, &fillRect);
}

void Graphics::renderText(int x, int y, const std::string &text) const {
    _font->text(x,y,text);
}

TextureRegion Graphics::getTileSprite(int ix, int iy) {
    if (ix < 0 || iy < 0 || ix >= 16 || iy >= 16) return tileset[0];
    return tileset[ix + iy*16];
}

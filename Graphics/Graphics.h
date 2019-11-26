//
// Created by luis.reis on 25-03-2018.
//

#ifndef ORESCLONE_GRAPHICS_H
#define ORESCLONE_GRAPHICS_H

#include <SDL.h>
#include <iostream>
#include <unordered_map>
#include <memory>
#include "TextureRegion.h"
#include "Font.h"

/**
 * Wrapper class that holds SDL Rendering state,
 * implements utility functions around SDL calls
 * and manages lifetime of graphical objects (textures and fonts).
 */
class Graphics {

private:
    /**
     * State variables that hold the result of several SDL initializations.
     * These will be used in ~Graphics() to free the allocated resources.
     * _sdlInit         - Whether the call to SDL_Init was successful.
     * _imgInit         - Whether the call to IMG_Init was successful.
     * _window          - Window created with SDL_CreateWindow.
     *                      If not null, window creation was successful and needs to be freed.
     * _renderer        - Renderer created with SDL_CreateRenderer.
     *                      If not null, renderer creation was successful and needs to be freed.
     */
    bool _sdlInit = false;
    bool _imgInit = false;
    SDL_Window *_window = nullptr;
    SDL_Renderer *_renderer = nullptr;

    /**
     * State of a virtual window used to abstract screen resolution.
     * This window is always SCREEN_WIDTH x SCREEN_HEIGHT.
     * _virtualWindow   - Texture used as render target when rendering normally.
     * _viewport        -
     */
    SDL_Texture*_virtualWindow = nullptr;

    /**
     * Destination rectangle where the virtual window will be rendered on the actual window.
     * Calculated with Graphics::updateViewport() at startup and at resize events.
     */
    SDL_Rect _viewport;

    /**
     * Textures storage indexed by name.
     */
    std::unordered_map<std::string, SDL_Texture*> _textures;

    /**
     * Font used to render text. \see Font.
     */
    std::unique_ptr<Font> _font;

    /**
     * Matrix of 256 16x16 textures created from a texture atlas named "tileset".
     */
    std::vector<TextureRegion> tileset;

    /**
     * Actual window width and height in pixels.
     */
    int window_width, window_height;

public:

    /**
     * Initialization function for all private fields of this class.
     * See the respective docs.
     *
     * @return
     *      0 if the initialization is successful.
     *      An error number >0 otherwise.
     */
    int init();

    /**
     * Destructor for this class.
     * Frees all initialized assets that were allocated on init,
     * even if init is not successful.
     */
    virtual ~Graphics();

    /**
     * Loads all textures required by the game into storage.
     * Each texture has a name handle.
     * Textures paths and names are hardcoded in this function.
     *
     * @return
     *      0 if all textures are loaded correctly.
     *      1 if there is an error loading one of them.
     */
    int loadTextures();

    /**
     * Frees all loaded textures even if loadTextures has failed.
     */
    void destroyTextures();

    /**
     * Loads a texture from a file.
     *
     * @param file Path to the image file to load into the texture.
     * @return
     *      nullptr if the loading is not successful.
     *      A pointer to a SDL_Texture otherwise.
     */
    SDL_Texture* loadTexture(const std::string &file);

    /**
     * Gets a loaded texture by name.
     * \see Graphics::loadTextures().
     *
     * @param name Name of the texture to load.
     * @return
     *      Pointer to the texture if it exists.
     *      nullptr otherwise.
     */
    SDL_Texture* getTextureByName(const std::string &name) const;

    /**
     * Renders a texture region on screen given a position and angle.
     * Texture region center is its top left corner.
     * The texture is rotated angle radians around the region's center.
     *
     * @param texture Texture to render.
     * @param x x position in virtual screen coordinates.
     * @param y y position in virtual screen coordinates.
     */
    void renderTextureRegion(const TextureRegion &textureRegion, int x, int y, float angle = 0) const;

    /**
     * Renders a given text left-aligned on position (x,y)
     *
     * @param x x position in virtual screen coordinates.
     * @param y y position in virtual screen coordinates.
     * @param text Text to be rendered.
     */
    void renderText(int x, int y, const std::string& text) const;

    /**
     * Sets renderer draw color using SDL_SetRenderDrawColor.
     *
     * @param color Color to set in the format RGBA8.
     */
    void setColor(int color, float alpha = 1);

    /**
     * Fills a rectangle on screen using the renderer's color.
     *
     * @param x x position in virtual screen coordinates.
     * @param y y position in virtual screen coordinates.
     * @param w Width of the rectangle in virtual screen pixels.
     * @param h Height of the rectangle in virtual screen pixels.
     */
    void rect(int x, int y, int w, int h);

    /**
     * Sets the render target to the virtual window texture
     * and clears it black.
     */
    void clear();

    /**
     * Sets the render target to the actual window, clears
     * it black and renders the virtual window to its viewport.
     */
    void present();

    /**
     * Updates the viewport for the virtual window.
     * This is done via letterboxing or pillarboxing to ensure the aspect ratio is correct.
     * Called on startup and on window resize events.
     *
     * @param ww Actual width of the window in pixels.
     * @param wh Actual height of the window in pixels.
     */
    void updateViewport(int ww, int wh);


    /**
     * Converts window coordinates to virtual window coordinates.
     * Used to position mouse events.
     *
     * @param wx x coordinate in actual window space.
     * @param wy y coordinate in actual window space.
     * @param x Reference to x coordinate in virtual window space.
     * @param y Reference to y coordinate in virtual window space.
     * @return Returns by reference.
     */
    void windowToGameCoords(int wx, int wy, int &x, int &y)const;

    /**
     * Gets a sprite from the tileset.
     *
     * @param ix x index of the sprite in the tileset.
     * @param iy y index of the sprite in the tileset.
     * @return
     *      A texture region for the sprite if valid coordinates are given.
     *      The first sprite of the tileset otherwise.
     */
    TextureRegion getTileSprite(int ix, int iy);
};

#endif

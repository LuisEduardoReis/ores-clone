//
// Created by luis.reis on 01-04-2018.
//

#include "Input.h"
#include "Game.h"

using namespace std;

void Input::handleWindowEvent(SDL_Event &e) {
    switch(e.window.event) {
        case SDL_WINDOWEVENT_MOVED:
            //printf("Window moved to %d, %d\n", e.window.data1, e.window.data2);
            break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            //printf("Window resized to %d, %d\n", e.window.data1, e.window.data2);
            game.graphics->updateViewport(e.window.data1, e.window.data2);
            break;
        default:break;
    }
}

void Input::updateMousePosition(int screen_mx, int screen_my) {
    game.graphics->windowToGameCoords(screen_mx, screen_my, mouse_x, mouse_y);
}

void Input::handleMouseEvent(SDL_Event &e) {
    switch(e.type) {
        case SDL_MOUSEBUTTONDOWN:
            updateMousePosition(e.button.x, e.button.y);
            switch (e.button.button) {
                case SDL_BUTTON_LEFT:
                    mouse_left = true;
                    if (game.level != nullptr) game.level->click(mouse_x, mouse_y);
                    break;
                case SDL_BUTTON_RIGHT: mouse_right = true; break;
                default:break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            updateMousePosition(e.button.x, e.button.y);
            switch (e.button.button) {
                case SDL_BUTTON_LEFT: mouse_left = false; break;
                case SDL_BUTTON_RIGHT: mouse_right = false; break;
                default:break;
            }
            break;
        case SDL_MOUSEMOTION:
            updateMousePosition(e.motion.x, e.motion.y);
            break;
        default:break;
    }
}


void Input::handleKeyEvent(SDL_Event &e) {
    if (e.key.keysym.scancode == SDL_SCANCODE_R) {
        game.restart();
    }
}

void Input::handle() {
    SDL_Event event{};
    while(SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                game.stop();
                break;
            case SDL_MOUSEBUTTONUP:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEMOTION:
                handleMouseEvent(event);
                break;
            case SDL_WINDOWEVENT:
                handleWindowEvent(event);
                break;
            case SDL_KEYDOWN:
                handleKeyEvent(event);
                break;
            default:break;
        }
    }
}

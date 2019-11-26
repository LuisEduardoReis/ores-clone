//
// Created by luis.reis on 01-04-2018.
//

#ifndef ORESCLONE_INPUT_H
#define ORESCLONE_INPUT_H


#include "Graphics/Graphics.h"
class Game;


class Input {

private:
    Game& game;

public:
    int mouse_x = 0, mouse_y = 0;
    bool mouse_left = false, mouse_right = false;
    bool mouse_left_p = false, mouse_right_p = false;

    explicit Input(Game& game) : game(game) {};

public:
    void handleMouseEvent(SDL_Event &e);

    void handleWindowEvent(SDL_Event &e);

    void handleKeyEvent(SDL_Event &e);

    void updateMousePosition(int screen_mx, int screen_my);

    void handle();
};


#endif //ORESCLONE_INPUT_H

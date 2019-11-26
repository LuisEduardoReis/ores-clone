#include <iostream>
#include "Game.h"

using namespace std;

int main(int, char**) {

    Game game;

    while(game.isRunning()) {
        game.input->handle();
        game.tick();
        game.render();
    }

    return 0;
}
#include "Game.h"
#include "Utility.h"

int main(int argc, char *argv[]) {
    Game game;
    game.init();
    game.loadMedia();
    while (!game.isDone()) {
        game.handleInput();
        game.update();
        game.render();
    }
    game.close();

	return 0;
}
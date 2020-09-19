#include "Apple.h"

Apple::Apple() {
    respawn();
};
Apple::~Apple() {
    mAppleTexture.free();
};

bool Apple::loadMedia(SDL_Renderer *lRenderer) {
    if (!mAppleTexture.loadFromFile("Apple.png", lRenderer)) {
        printf("Failed to load apple texture!\n");
        return false;
    }
    return true;
}

Vector2i Apple::getPosition() {
    return mPosition;
}

void Apple::respawn() {
    int maxX = (SCREEN_WIDTH / BLOCK_SIZE) - 2;
    int maxY = (SCREEN_HEIGHT / BLOCK_SIZE) - 2;
    mPosition = Vector2i(rand() % maxX + 1, rand() % maxY + 1);
}

void Apple::render(SDL_Renderer *lRenderer, float *scale) {
    mAppleTexture.render(mPosition.x * BLOCK_SIZE, mPosition.y * BLOCK_SIZE,
                         lRenderer, NULL, scale);
}
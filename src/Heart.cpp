#include "Heart.h"

LTimer Heart::mFrameTimer;
int Heart::mFrame;

Heart::Heart(int x, int y) : mPosition(x, y) {
    if (!mFrameTimer.isStarted()) {
        mFrameTimer.start();
    }
}

Heart::~Heart() {
    mHeartTextureSheet.free();
}

void Heart::update() {
    if (mFrameTimer.getTicks() >= FRAME_DURATION) {
        mFrame++;
        mFrameTimer.start();
        if (mFrame >= FRAME_COUNT) {
            mFrame = 0;
        }
    }
}

bool Heart::loadMedia(SDL_Renderer *lRenderer) {
    if (!mHeartTextureSheet.loadFromFile("Heart.png", lRenderer)) {
        printf("Failed to load heart texture!\n");
        return false;
    }

    mHeartClips[0] = {0, 0, 16, 16};
    mHeartClips[1] = {16, 0, 16, 16};

    return true;
}

void Heart::render(SDL_Renderer *lRenderer, float *lScale) {
    mHeartTextureSheet.render(mPosition.x * BLOCK_SIZE,
                              mPosition.y * BLOCK_SIZE, lRenderer,
                              &mHeartClips[mFrame], lScale);
}